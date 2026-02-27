/*
2026-02-24
epoll기반 에코 서버 (Modern C++)
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<stdexcept>// std::runtime_error 사용을 위해
#include<unistd.h>
#include<arpa/inet.h>// sockaddr_in, htonl, htons
#include<sys/epoll.h>// epoll_create, epoll_ctl, epoll_wait, epoll_event
#include"fd_guard.h"

int main(int argc, char *argv[])
{
  const int buf_size = 100;// 수신 버퍼 크기
  const int epoll_size = 50;// epoll이 한 번에 감시할 최대 이벤트 수

  if(argc != 2)//포트 번호 인자 확인
  {
    std::cerr << "usage: " << argv[0] << "<port>" <<std::endl;
    exit(1);
  }

  try
  {
    // TCP 소켓 생성, 실패 시 Fd_guard 생성자에서 예외 던짐
    Fd_guard serv_sock(socket(PF_INET, SOCK_STREAM, 0));

    sockaddr_in serv_adr{};// {}로 구조체 전체 0 초기화 (memset 대체)
    serv_adr.sin_family = AF_INET;//IPv4
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(std::stoi(argv[1]));

    // SO_REUSEADDR: 서버 재시작 시 "Address already in use" 에러 방지
    int opt = 1;
    setsockopt(serv_sock.get(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 소켓에 IP/포트 바인딩
    if(bind(serv_sock.get(),(sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
    throw std::runtime_error("bind error");

    // 연결 대기 상태로 전환, 대기열 최대 5개
    if(listen(serv_sock.get(),5) == -1)
    throw std::runtime_error("listen error");

    // epoll 인스턴스 생성, 실패 시 예외
    Fd_guard epfd(epoll_create(epoll_size));
    // 이벤트 목록을 vector로 관리 (malloc 대신)
    std::vector<epoll_event>ep_events(epoll_size);

    epoll_event event{};// 등록할 이벤트 구조체 0 초기화
    event.events = EPOLLIN;// 읽기 이벤트 감시
    event.data.fd = serv_sock.get();// 감시할 fd = 서버 소켓
    // epoll에 서버 소켓 등록
    epoll_ctl(epfd.get(), EPOLL_CTL_ADD, serv_sock.get(), &event);

    std::vector<char> buf(buf_size);

    while(true)
    {
      // 이벤트 발생까지 블로킹 대기 (-1 = 무한 대기)
      // 반환값 = 발생한 이벤트 수
      int event_cnt = epoll_wait(epfd.get(), ep_events.data(), epoll_size, -1);
      if(event_cnt == -1)
      throw std::runtime_error("epoll_wait error");

      for(int i = 0; i<event_cnt; i++)//발생한 이벤트 순회
      {
        if(ep_events[i].data.fd == serv_sock.get())//// 이벤트 fd = 서버 소켓 → 새 클라이언트 접속
        {
          sockaddr_in clnt_adr{};
          socklen_t adr_sz = sizeof(clnt_adr);
          //글라이언트 연결 수락, 새 fd 반환
          int clnt_fd = accept(serv_sock.get(),(sockaddr*)&clnt_adr, &adr_sz);
          if(clnt_fd == -1)
          throw std::runtime_error("accept error");

          event.events = EPOLLIN;//읽기 이벤트 감시
          event.data.fd = clnt_fd;//감시할 fd = 클라이언트 소켓
          // epoll에 클라이언트 소켓 등록
          epoll_ctl(epfd.get(), EPOLL_CTL_ADD, clnt_fd, &event);
          std::cout << "connected client: " << clnt_fd << std::endl;
        }
        else// 이벤트 fd = 클라이언트 소켓 → 데이터 수신
        {
          int fd = ep_events[i].data.fd;
          // 데이터 수신, recv는 소켓 전용 함수 (read보다 옵션 지정 가능)
          int str_len = recv(fd, buf.data(), buf_size, 0);

          if(str_len == 0)// 수신 바이트 0 = 클라이언트가 연결 종료
          {
            //epoll감시 목록에서 제거
            epoll_ctl(epfd.get(), EPOLL_CTL_DEL, fd, nullptr);// nullptr = C++ 스타일 NULL
            close(fd);//socket닫기
            std::cout << "closed client: " << fd << std::endl;
          }
          else if(str_len > 0)//정상 수신
          {
            // 받은 데이터 그대로 에코 (send는 소켓 전용 함수)
            send(fd, buf.data(), str_len, 0);
          }
          else// str_len == -1
          {
              epoll_ctl(epfd.get(), EPOLL_CTL_DEL, fd, nullptr);
              close(fd);
              std::cerr << "recv error, closed client: " << fd << std::endl;
          }
        }
      }
    }
  }
  catch(const std::exception& e)// 모든 std::exception 계열 예외 처리
  {
    std::cerr << "오류: " << e.what() << std::endl;
    exit(1);
  }
  
  return 0;
}