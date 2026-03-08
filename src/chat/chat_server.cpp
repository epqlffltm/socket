/*
2026-03-04
chat-server
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<unistd.h>
#include<stdexcept>// std::runtime_error 사용을 위해
#include<arpa/inet.h>// sockaddr_in, htonl, htons
#include<sys/epoll.h>// epoll_create, epoll_ctl, epoll_wait, epoll_event

//전역 변수
const int buf_size = 100;// 수신 버퍼 크기
const int max_clnt = 256;

int clnt_cnt = 0;
//int clnt_socks[256];c 스타일
std::vector<int>clnt_socks;//c++ 스타일
std::mutex mutx;

//함수
void handle_clnt(int clnt_sock);
void send_msg(char *msg, int len);
//void error_handling(const char *message);

int main(int argc, char *argv[])
{
  //const int buf_size 100;// 수신 버퍼 크기
  //const max_clnt 256;

  /*if(argc != 2)//포트 번호 인자 확인
  {
    std::cerr << "usage: " << argv[0] << "<port>" << std::endl;
    exit(1);
  }*/

  try
  {
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    serv_sock = socket(PF_INET, SOCK_STREAM,0);
    //std::thread t_id;
    if(serv_sock == -1)//포트 번호 인자 확인
    throw std::runtime_error("socket error");
    /*{
      std::cerr << "usage: " << argv[0] << "<port>" << std::endl;
      exit(1);
    }*/
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(opt));


    if(bind(serv_sock,(sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
    throw std::runtime_error("bind error");

    if(listen(serv_sock,5) == -1)
    throw std::runtime_error("listen error");

    while(true)
    {
      clnt_adr_sz = sizeof(clnt_adr);
      clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
      if(clnt_sock == -1)
      throw std::runtime_error("accrpt error");

      std::lock_guard<std::mutex>lock(mutx);
      clnt_socks.push_back(clnt_sock);
      clnt_cnt++;

      //thread생성후 detacj
      std::thread t(handle_clnt, clnt_sock);
      t.detach();

      //std::thread_mutex_lock(&mutx);
      //std::thread_create(&t_id,NULL,handle_clnt,(void*)&clnt_sock);
      //std::thread_detach(t_id);
      std::cout << "connected client IP: " << inet_ntoa(clnt_adr.sin_addr) << std::endl;
    }

    close(serv_sock);
  }
  catch(const std::exception& e)
  {
    std::cerr << "error" << e.what() << std::endl;
    exit(1);
  }
  return 0;
}

void handle_clnt(int clnt_sock)
{
  char msg[buf_size];//c 스타일
  //std::string msg;//c++ 스타일
  int str_len = 0;

  while((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
  send_msg(msg,str_len);

  std::lock_guard<std::mutex> lock(mutx);
  for(int i = 0; i<clnt_cnt; i++)
  {
    if(clnt_sock == clnt_socks[i])
    {
      while(i++ < clnt_cnt -1)
      clnt_socks[i] = clnt_socks[i + 1];
      break;
    }
  }
  clnt_cnt--;
  close(clnt_sock);
}

void send_msg(char *msg, int len)
{
  std::lock_guard<std::mutex> lock(mutx);
  for(int i = 0; i < clnt_cnt; i++)
  {
    write(clnt_socks[i],msg,len);
  }
  //std::thread_mutex_unlock(&mutx);
}

/*void error_handling(const char *message)
{
    std::cerr << message << " error: " << strerror(errno) << std::endl;
    exit(1);
}*/