/*
2026-02-16
에코 클라이언트 개선판 (Modern C++)
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<stdexcept> // std::runtime_error
#include<unistd.h>// close()
#include<arpa/inet.h>// sockaddr_in, inet_addr, htons
#include<sys/socket.h>// socket, connect, send, recv
#include"fd_guard.h"

// RAII 패턴: 소켓 fd 자동 close
// 예외 발생해도 소멸자에서 close() 보장

int main(int argc, char *argv[])
{
  const int buf_size = 1024;

  if(argc != 3)
  {
    std::cerr << "usage: " << argv[0] << "<IP> <port>" << std::endl;
    exit(1);
  }

  try
  {
    // TCP 소켓 생성, 실패 시 FdGuard 생성자에서 예외
    Fd_guard sock(socket(PF_INET, SOCK_STREAM, 0));

    sockaddr_in serv_adr{};// {} 로 전체 0 초기화
    serv_adr.sin_family = AF_INET;// IPv4
    if(inet_pton(AF_INET, argv[1], &serv_adr.sin_addr) <= 0)
    throw std::runtime_error("잘못된 IP 주소");
    serv_adr.sin_port = htons(std::stoi(argv[2]));// 포트 설정

    // 서버에 연결 요청
    if(connect(sock.get(), (sockaddr*)&serv_adr, sizeof(serv_adr))  == -1)
    throw std::runtime_error("connect error");

    std::cout << "connected to server!" << std::endl;

    std::string input_msg;// 키보드 입력 저장
    std::vector<char>message(buf_size);// 서버 수신 데이터 버퍼

    while(true)
    {
      std::cout << "input message " << std::endl;
      std::cout << "q to quit" << std:: endl;
      std::getline(std::cin, input_msg);// 한 줄 입력 (공백 포함)


      if(input_msg == "q" || input_msg == "Q")// 종료 조건
      break;

      //서버로 메시지 전송
      send(sock.get(), input_msg.c_str(), input_msg.length(), 0);

      int recv_len = 0;
      // size_t → int 암묵적 변환 경고 방지
      int send_len = static_cast<int>(input_msg.length());

      // TCP 특성상 한 번에 다 못 받을 수 있으므로
      // 보낸 만큼 다 받을 때까지 반복
      while(recv_len < send_len)
      {
        // 이미 받은 만큼 포인터 이동해서 이어서 저장
        int recv_cnt = recv(sock.get(), message.data()+recv_len, buf_size - recv_len - 1, 0);

        if(recv_cnt == -1)
        throw std::runtime_error("recv error");
        
        if(recv_cnt == 0)// 서버가 연결 종료
        break;

        recv_len += recv_cnt;// 누적 수신 바이트 갱신
      }
      message[recv_len] = '\0';// 문자열 끝 표시
      std::cout << "message from server: " << message.data() << std::endl;
    }
  }
  catch(const std::exception& e)
  {
    std::cerr << "error: " << e.what() << std::endl;
    exit(1);
  }
  return 0;
}
