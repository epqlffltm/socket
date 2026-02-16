/*
2026-02-16
에코 클라이언트 개선판
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 1024;
  std::vector<char> message(buf_size);
  std::string input_msg;  // 입력용 메시지
  struct sockaddr_in serv_adr;

  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
    exit(1);
  }

  // 소켓 생성
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1)
  {
    error_handling("socket");
  }

  // 서버 주소 설정
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  // 연결
  if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("connect");
  }
  else
  {
    std::cout << "Connected to server!" << std::endl;
  }

  // 에코 루프
  while (true)
  {
    std::cout << "Input message (q to quit): ";
    std::getline(std::cin, input_msg);

    // 종료 조건
    if (input_msg == "q" || input_msg == "Q")
    {
      break;
    }

    // 전송
    write(sock, input_msg.c_str(), input_msg.length());

    // 수신 (개선: 전체 메시지 받을 때까지 반복)
    int recv_len = 0;
    int send_len = input_msg.length();
    
    while (recv_len < send_len)
    {
      int recv_cnt = read(sock, message.data() + recv_len, buf_size - recv_len - 1);
      if (recv_cnt == -1)
      {
        error_handling("read");
      }
      else if (recv_cnt == 0)  // 연결 종료
      {
        break;
      }
      recv_len += recv_cnt;
    }

    message[recv_len] = '\0';  // null terminator
    std::cout << "Message from server: " << message.data() << std::endl;
  }

  close(sock);
  
  return 0;
}