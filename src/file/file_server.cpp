/*
2026-02-22
파일 기반 서버.
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 30;
  int serv_sd = -1;
  int clnt_sd = -1;
  int opt = 1;
  FILE *fp;
  std::vector<char>message(buf_size);
  int read_cnt = -1;

  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if(argc != 2)
  {
    std::cerr << "useage: " <<argv[0] << "<port>" << std::endl;
    exit(1);
  }

  if((fp = fopen("test.txt","rb")) == NULL)
  {
    error_handling("fopen");
  }

  serv_sd = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sd == -1)
  {
    error_handling("socket");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  setsockopt(serv_sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("bind");
  }
  
  if(listen(serv_sd, 5) == -1)
  {
    error_handling("listen");
  }

  clnt_adr_sz = sizeof(clnt_adr);
  if((clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz)) == -1)
  {
    error_handling("accept");
  }

  while(true)
  {
    read_cnt = fread((void*)message.data(),1,buf_size,fp);
    if(read_cnt < buf_size)
    {
      write(clnt_sd, message.data(), read_cnt);
      break;
    }
    write(clnt_sd, message.data(), buf_size);
  }
  shutdown(clnt_sd,SHUT_WR);//출력 스크림 종료
  
  read(clnt_sd,message.data(),buf_size);
  
  fclose(fp);
  close(serv_sd);
  close(clnt_sd);

  return 0;
}