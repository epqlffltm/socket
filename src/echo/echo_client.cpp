/*
2026-02-15
에코 클라이언트
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  int str_len = -1;
  const int buf_size = 1024;
  std::vector<char> message(buf_size);//vector 버퍼
  std::string msg;//입력용 메세지
  struct sockaddr_in serv_adr;

  if(argc != 3)
  {
    std::cerr << "usage: " << argv[0] << "<ip>" << "<port>" << std::endl;
    exit(1);
  }

  int sock = socket(PF_INET, SOCK_STREAM, 0);//소켓 생성
  if(sock == -1)
  {
    error_handling("sock");
  }

  //server 주소 설정
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;//IPv4
  serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  // 연결
  if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("connect");
  }
  else
  {
    std::cout << "connected to server!" << std::endl;
  }

  while(true)//에코 루프
  {
    std::cout << "Input message (q to quit): ";
    std::getline(std::cin, msg);

    if(msg == "q" || msg == "Q")//종료 조건
    {
      break;
    }

    //전송
    write(sock, msg.c_str(), msg.length());

    //수신
    str_len = read (sock, message.data(), message.size() - 1);
    if (str_len == -1)
    {
      error_handling("read");
    }

    message[str_len] = '\0';// null terminator

    std::cout << "message : " << message.data() << std::endl;
  }
  close(sock);

  return 0;
}