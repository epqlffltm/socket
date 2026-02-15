/*
2026-02-15
에코 서버
*/

#include<iostream>
#include<cstdlib.h>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  int clnt_sock = -1, str_len, opt = 1;
  const int buf_size = 1024;
  std::vector<char>message(buf_size);

  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if(argc != 2)
  {
    std::cerr << "usage: " << argv[0] << "<port>" << std::endl;
    exit(1);
  }

  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);//소켓 생성
  if(serv_sock == -1)
  {
    error_handling("socket() error");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));//주소 구조체 활성화
  serv_adr.sin_family = AF_INET;//IPv4
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);//내컴퓨터 ip할당
  serv_adr.sin_port = htons(atoi(argv[1]));//포트번호 할당

  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("bind");
  }

  if(listen(serv_sock, 5) == -1)
  {
    error_handling("listen");
  }

  clnt_adr_sz = sizeof(clnt_adr);

  for(int i = 0; i < 5; i++)
  {
    clnt_sock = accept(serv_sock, (struct spcladdr*)&clnt_adr, &clnt_adr_sz);
    if(clnt_sock == -1)
    {
      error_handling("accept");
    }
    else
    {
      std::cout << "connected client" << i + 1 << std::endl;
    }
    while((str_len = read(clnt_sock, message.data(), buf_size)) != 0)
    {
      write(clnt_sock, message.data(), str_len);
    }
    close(clnt_sock);
  }
  close(serv_sock);

  return 0;
}