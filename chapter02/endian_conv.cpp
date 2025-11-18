#include<iostream>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
  unsigned short host_port=0x1234;
  unsigned short net_port;
  unsigned long host_addr=0x12345678;
  unsigned long net_addr;

  net_port=htons(host_port);
  net_addr=htonl(host_addr);

  std::cout << std::showbase << std::hex;

  std::cout<<"host ordered port: "<<host_port<<std::endl;
  std::cout<<"network ordered port: "<<net_port<<std::endl;
  std::cout<<"host ordered address: "<<host_addr<<std::endl;
  std::cout<<"neetwork ordered address: "<<net_port<<std::endl;

  return 0;
}