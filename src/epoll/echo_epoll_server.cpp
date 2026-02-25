/*
2026-02-24
epoll기반 에코 서버
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  const int buf_size = 100;
  const int epoll_size = 50;
  int serv_sock = -1;
  int clnt_sock = -1;
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t adr_sz;
  int str_len = -1;
  std::vector<char>buf(buf_size);

  struct epoll_event *ep_events;
  struct epoll_event event;
  int epfd, event_cnt;

  if(argc != 2)
  {
    std::cerr << "usage: " << argv[0] << "<port>" << std::endl;
    exit(1);
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));

  if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))== -1)
  error_handling("bind");

  if(listen(serv_sock, 5) == -1)
  error_handling("listen");

  epfd = epoll_create(epoll_size);
  ep_events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*epoll_size);

  event.events=EPOLLIN;
  event.data.fd = serv_sock;
  epoll_ctl(epfd,EPOLL_CTL_ADD, serv_sock, &event);

  while(true)
  {
    event_cnt = epoll_wait(epfd, ep_events, epoll_size, -1);
    if(event_cnt == -1)
    {
      std::cerr << "epoll_waif() error" << std::endl;
      break;
    }
    for(int i = 0; i<event_cnt; i++)
    {
      if(ep_events[i].data.fd == serv_sock)
      {
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr, &adr_sz);
        event.events = EPOLLIN;
        event.data.fd = clnt_sock;
        epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock,&event);
        std::cout << "connected client : " << clnt_sock << std::endl;
      }
      else
      {
        str_len = read(ep_events[i].data.fd, buf.data(),buf_size);
        if(str_len == 0)
        {
          epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
          close(ep_events[i].data.fd);
          std::cout << "closed client : " << ep_events[i].data.fd << std::endl;
        }
        else
        write(ep_events[i].data.fd,buf.data(),str_len);
      }
    }
  }
  close(serv_sock);
  close(epfd);

  return 0;
}
