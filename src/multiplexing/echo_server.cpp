/*
2026-02-23
멀티플렉스 서버 구축
*/

#include<iostream>
#include<cstdlib>// atoi
#include<cstring>// memset
#include<string>// std::string
#include<vector> // std::vector
#include<unistd.h>// read, write, close
#include<arpa/inet.h>// htonl, htons, inet_addr
#include<sys/socket.h>// socket, bind, listen, accept
#include<sys/time.h>// timeval 구조체
#include<sys/select.h>// select, fd_set, FD_ZERO, FD_SET, FD_ISSET, FD_CLR
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 100;// 수신 버퍼 크기
  int serv_sock = -1;// 서버 소켓 fd
  int clnt_sock = -1;// 클라이언트 소켓 fd
  struct sockaddr_in serv_adr, clnt_adr;// 서버/클라이언트 주소 구조체
  struct timeval timeout;// select 타임아웃 시간
  fd_set reads, cpy_reads;// 감시할 fd 집합 / 매 루프마다 복사본

  socklen_t adr_sz;// 클라이언트 주소 구조체 크기
  int fd_max = -1;  // 현재 등록된 fd 중 최댓값
  int str_len = -1;  // 현재 등록된 fd 중 최댓값
  int fd_num = -1;  // 현재 등록된 fd 중 최댓값

  std::vector<char>message(buf_size);  // 현재 등록된 fd 중 최댓값

  if(argc != 2)
  {
    std::cerr << "usage " << argv[0] << " " <<"<port>" <<std::endl;
    exit(1);
  }

  if((serv_sock = socket(PF_INET, SOCK_STREAM,0)) == -1)  // TCP 소켓 생성
  error_handling("socket");

  memset(&serv_adr,0,sizeof(serv_adr));  // 구조체 0으로 초기화
  //std::vector<char>buf(1024,0);
  serv_adr.sin_family = AF_INET;  // IPv4
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 IP에서 접속 허용
  serv_adr.sin_port = htons(atoi(argv[1]));  // 포트 설정 (호스트->네트워크 바이트 순서 변환)

  if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)  // 소켓에 IP/포트 할당
  error_handling("bind");

  if(listen(serv_sock,5) == -1)  // 연결 대기 상태로 전환, 대기 큐 5
  error_handling("listen");

  FD_ZERO(&reads);  // fd 집합 초기화 (모든 비트 0)
  FD_SET(serv_sock, &reads);  // 서버 소켓을 감시 대상에 등록
  fd_max = serv_sock;  // 현재 최대 fd = 서버 소켓

  while(true)
  {
    cpy_reads = reads;    // select는 fd_set을 변경하므로 원본 보존용 복사
    timeout.tv_sec = 5;    // 타임아웃 5초
    timeout.tv_usec = 5000;    // + 5000 마이크로초

    if((fd_num = select(fd_max+1, &cpy_reads,0,0,&timeout)) == -1)
    // 이벤트 발생한 fd 수 반환, 타임아웃이면 0, 오류면 -1
    break;
    if(fd_num == 0)    // 타임아웃 - 이벤트 없음
    continue;

    for(int i = 0; i<fd_max+1; i++)    // 모든 fd 순회
    {
      if(FD_ISSET(i, &cpy_reads) )      // 해당 fd에 이벤트 발생했는지 확인
      {
        if(i == serv_sock)        // 서버 소켓 이벤트 = 새 클라이언트 접속
        {
          adr_sz = sizeof(clnt_adr);          // 클라이언트 연결 수락
          clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &adr_sz);
          // 새 클라이언트 fd를 감시 대상에 추가
          FD_SET(clnt_sock, &reads);
          if(fd_max < clnt_sock)
          fd_max = clnt_sock;// 최대 fd 갱신
          std::cout << "connected client: " << clnt_sock << std::endl;
        }
        else// 클라이언트 소켓 이벤트 = 데이터 수신 또는 연결 종료
        {
          str_len = read(i,message.data(),buf_size);// 데이터 수신
          if(str_len == 0)// 수신 바이트 0 = 클라이언트 연결 종료
          {
            FD_CLR(i, &reads);// 감시 대상에서 제거
            close(i);// 소켓 닫기
            std::cout << "closed client: " << i << std::endl;
          }
          else
          {
            write(i,message.data(),str_len);// 받은 데이터 그대로 에코(echo) 전송
          }
        }
      }
    }
  }
  close(serv_sock);// 서버 소켓 닫기

  return 0;
}