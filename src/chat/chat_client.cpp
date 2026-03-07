/*
2026-03-05
chat-server-client
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<thread>
//#include<mutex>
#include<unistd.h>
#include<stdexcept>// std::runtime_error 사용을 위해
#include<arpa/inet.h>// sockaddr_in, htonl, htons
#include<sys/epoll.h>// epoll_create, epoll_ctl, epoll_wait, epoll_event

//전역 변수.
const int buf_size = 100;
const int name_size = 20;

//함수
void send_msg(int *sock);
void recv_msg(int *sock);
//void error_handling(char *msg);

char name[name_size] = "[DEFAULT]";
//std::vector<char>msg(buf_size);

int main(int argc, char *argv[])
{
  int sock = -1;
  struct sockaddr_in serv_addr;

  //std::tread snd_thread, rcv_thread;
  //void thread_return 0;

  try
  {
    if(argc != 4)
    {
      throw std::runtime_error("usage: <IP> <port> <name>");
    }
    sprintf(name,"[%s]", argv[3]);
    if((sock = socket(PF_INET, SOCK_STREAM,0)) == -1)
    {
      throw std::runtime_error("socket error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
      throw std::runtime_error("connect error");
    }

    std::thread snd_thread(send_msg, &sock);
    std::thread rcv_thread(recv_msg, &sock);
    
    snd_thread.join();
    rcv_thread.join();

    close(sock);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  return 0;
}

void send_msg(int* sock)
{
  //int sock = *((int*)arg);
  //char name_mag[name_size+buf_size]
  //std::vector<char>name_msg(name_size+buf_size);
  std::string name_msg;
  std::string input;

  while(true)
  {
    //fgets(msg.data(),buf_size,stdin);
    std::getline(std::cin, input);

    if(input == "q" || input == "Q")
    {
      close(*sock);
      exit(0);
    }
    //sprintf(name_msg.data(), "%s %s", name, msg.data());
    name_msg = std::string(name) + " " + input + "\n";
    write(*sock, name_msg.c_str(), name_msg.size());
  }
}

void recv_msg(int* sock)
{
  //int sock = (int*)arg;
  std::string name_msg(name_size + buf_size, '\0');
  int str_len = -1;

  while(true)
  {
    if((str_len = read(*sock, &name_msg[0], name_size + buf_size - 1)) ==-1)
    //if(str_len == -1)
    exit(1);

    name_msg[str_len] = 0;
    std::cout << name_msg;
  }
}
//void error_handling(char *msg);