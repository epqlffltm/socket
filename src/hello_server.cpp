/*
2026-02-14
hello server
*/
#include<iostream>
#include<cstdlib>
#include<cstring.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(const char *message);

int main(int argc, char *argv[])
{
  int serv_sock = -1, clnt_sock = -1;

  struct sockaddr_in serv_addr, clnt_addr;
  socklen_t clnt_addr_size;

  char message[] = "hallow wared!";

  if(argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if(serv_sock == -1)
  {
    error_handling("socket()");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  serv_addr.sin_port=htons(atoi(argv[1]));

  int opt = 1;
  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
  {
    error_handling("bind()");
  }

  if(listen(serv_sock, 5) == -1)
  {
    error_handling("listen()");
  }

  clnt_addr_size = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
  if(clnt_sock == -1)
  {
    error_handling("accept()");
  }

  send(clnt_sock, message, strlen(message), 0);

  close(clnt_sock);
  close(serv_sock);

  return 0;
}

void error_handling(const char *message)
{
  std::cerr << message << " error" << strerror(errno) << std::endl;
  exit(1);
}