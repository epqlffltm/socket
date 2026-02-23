/*
2026-02-23
멀티플렉스 서버 구축
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 100;
  int serv_sock = -1;
  int clnt_sock = -1;
  struct sockaddr_in serv_adr, clnt_adr;
  struct timeval timeout;
  fd_set reads, cpy_reads;

  socklen_t adr_sz;
  int fd_max = -1;
  int str_len = -1;
  int fd_num = -1;

  std::vector<char>message(buf_size);

  if(argc != 2)
  {
    std::cerr << "usage " << argv[0] << " " <<"<port>" <<std::endl;
    exit(1);
  }

  if((serv_sock = socket(PF_INET, SOCK_STREAM,0)) == -1)
  error_handling("socket");
  
  memset(&serv_adr,0,sizeof(serv_adr));
  //std::vector<char>buf(1024,0);
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  error_handling("bind");

  if(listen(serv_sock,5) == -1)
  error_handling("listen");

  FD_ZERO(&reads);
  FD_SET(serv_sock, &reads);
  fd_max = serv_sock;

  while(true)
  {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_user = 5000;

    if((fd_num = select(fd_max+1, &cpy_reads,0,0,&timeout)) == -1)
    break;
    if(fd_num == 0)
    continue;

    for(int i = 0; i<fd_max+1; i++)
    {
      if(FD_ISSET(i, &cpy_reads) )
      {
        if(i == serv_sock)
        {
          adr_sz = sizeof(clnt_adr);
          clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &adr_sz);
          FD_SET(clnt_sock, &reads);
          if(fd_max < clnt_sock)
          fd_max = clnt_sock;
          std::cout << "connected client: " << clnt_sock << std::endl;
        }
        else
        {
          str_len = read(i,message,buf_size);
          if(str_len == 0)
          {
            FD_CLR(i, &reads);
            close(i);
            std::cout << "closed client: " << i << std::endl;
          }
          else
          {
            write(i,message,str_len);
          }
        }
      }
    }
  }
  close(serv_sock);

  return 0;
}