/*
2026-03-04
chat-server + epoll
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<thread>
#include<mutex>
#include<algorithm>
#include<unistd.h>
#include<stdexcept>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>

const int buf_size = 100;
const int max_clnt = 256;
const int epoll_size = 50;

int clnt_cnt = 0;
std::vector<int> clnt_socks;
std::mutex mutx;

void handle_clnt(int clnt_sock);
void send_msg(char* msg, int len);

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    std::cout << "usage: " << argv[0] << "<port>" << std::endl;
    exit(1);
  }

  try
  {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if((serv_sock = socket(PF_INET, SOCK_STREAM,0))== -1)
    {
      throw std::runtime_error("socket error");
    }

    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
    {
      throw std::runtime_error("bind error");
    }

    if(listen(serv_sock, 5) == -1)
    throw std::runtime_error("listen error");

    int epfd = epoll_create(epoll_size);
    if(epfd == -1)
    {
      throw std::runtime_error("epoll_creare error");
    }

    struct epoll_event event;
    std::vector<struct epoll_event> ep_events(epoll_size);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock,&event);

    while(true)
    {
      int event_cnt = epoll_wait(epfd, ep_events.data(),epoll_size, -1);
      if(event_cnt == -1)
      throw std::runtime_error("epoll_wait error");

      for(int i = 0; i<event_cnt; i++)
      {
        if(ep_events[i].data.fd == serv_sock)
        {
          clnt_adr_sz = sizeof(clnt_adr);
          if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz)) == -1)
          {
            throw std::runtime_error("accept error");
          }

          event.events = EPOLLIN;
          event.data.fd = clnt_sock;
          epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);

          std::lock_guard<std::mutex>lock(mutx);
          clnt_socks.push_back(clnt_sock);
          clnt_cnt++;

          std::cout << "connectrd client IP: " << inet_ntoa(clnt_adr.sin_addr) << std::endl;
        }
        else
        {
          std::thread t(handle_clnt, ep_events[i].data.fd);
          t.detach();
        }
      }
    }
    close(serv_sock);
    close(epfd);

  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  return 0;
}

void handle_clnt(int clnt_sock)
{
  char msg[buf_size];  // read에 직접 쓰려면 char 배열이 편함
  int str_len = 0;

  str_len = read(clnt_sock, msg, buf_size);
  if(str_len <= 0)
  {
    // 클라이언트 종료
    std::lock_guard<std::mutex> lock(mutx);
    clnt_socks.erase(std::remove(clnt_socks.begin(), clnt_socks.end(), clnt_sock), clnt_socks.end());
    clnt_cnt--;
    close(clnt_sock);
    return;
  }

  send_msg(msg, str_len);
}

void send_msg(char* msg, int len)
{
  std::lock_guard<std::mutex> lock(mutx);
  for(int i = 0; i < clnt_cnt; i++)
  {
    write(clnt_socks[i], msg, len);
  }
}