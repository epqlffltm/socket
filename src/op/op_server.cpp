/*
2026-02-16
계산기 서버
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<vector>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  int clnt_sock = -1, str_len = 0, opt = 1, sum = 0;\
  const int buf_size = 1024;
  std::vector<char>message(buf_size);
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if(argc != 2)
  {
    std::cerr << "useage: " << argv[0] << "<port>" << std::endl;
    exit(1);
  }

  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1)
  {
    error_handling("socket");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));//주소 구조체 활성화
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("bond");
  }

  if(listen(serv_sock, 5) == -1)
  {
    error_handling("listen");
  }

  clnt_adr_sz = sizeof(clnt_adr);

  for(int i = 0; i < 5; i++)
  {
    sum = 0;//새 클라이언트마다 합계 초기화
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock == -1)
    {
      error_handling("accept");
    }
    else
    {
      std::cout << "connected client" << i + 1 << std::endl;
    }
    while((str_len = read(clnt_sock, message.data(), buf_size - 1)) > 0)
    // buf_size - 1을 해서 널 문자가 들어갈 자리를 남겨둔다.
    {
      write(clnt_sock, message.data(), str_len);
      message[str_len] = '\0';
      if((message[0] == 'q' || message[0] == 'Q'))// q가 들어오면 종료
      {
        std::string result = "Total Sum: " + std::to_string(sum) + "\n";
        write(clnt_sock, result.c_str(), result.length());
        break;
      }
      int val = atoi(message.data());
      sum += val;
      std::cout << "Received: " << val << ", Current Sum: " << sum << std::endl;
    }
    close(clnt_sock);
  }
  close(serv_sock);

  return 0;
}
