/*
2026-02-14
hello client
*/
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include "error_handling.h"

int main(int argc, char* argv[])
{
  int sock = -1, str_len = -1;
  char message[30];
  struct sockaddr_in serv_addr;

  if(argc != 3)
  {
    std::cerr << "usage:" << argv[0] << "<port>" << std::endl;
    exit(1);
  }

  serv = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    error_handling("socket error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
  serv_addr.sin_port=htons(atoi(argv[2]));

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  {
    error_handling("connect error");
  }

  str_len = read(sock, message, sizeof(message)-1);
  message[str_len] = '\0'
  if(str_len == -1)
  {
    error_handling("read error");
  }

  std::cout << message << std::endl;

  close(sock);

  return 0;
}