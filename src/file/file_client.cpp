/*
2026-02-22
파일 기반 클라이언트
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 30;
  int sd = -1;
  FILE *fp;

  std::vector<char>message(buf_size);
  int read_cnt = -1;
  struct sockaddr_in serv_adr;
  if(argc != 3)
  {
    std::cerr << "usage: " << argv[0] << "<IP>" << "<port>" << std::endl;
    exit(1);
  }

  if((fp = fopen("receive.dat","wb")) == NULL)
  {
    error_handling("fopen");
  }

  if((sd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
  {
    error_handling("socket");
  }

  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_adr.sin_port = htons(atoi(argv[2]));

  if(connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
  {
    error_handling("connect");
  }

  while(true)
  {
    if((read_cnt = read(sd, message.data(), buf_size)) == -1)
    {
      error_handling("read");
    }
    if(read_cnt == 0)//EOF
    {
      break;
    }
    fwrite((void*)message.data(),1,read_cnt,fp);
  }

  std::cout << "received file data" << std::endl;
  write(sd,"thank you", 10);

  fclose(fp);
  close(sd);

  return 0;
}