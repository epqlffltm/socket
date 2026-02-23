/*
2026-02-23
주소 가저오기
*/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<unistd.h>
#include<arpa/inet.h>
#include"../error_handling.h"

int main(int argc, char *argv[])
{
  struct hostnet *host;

  if(argc != 2)
  {
    std::cerr << "usage" << "<addr>" << argv[0] << std::endl;
    exit(1);
  }

  host = gethostbyname(argv[1]);
  if(!host)
  {
    error_handling("host");
  }

  std::cout << "official name : " << host->h_name << std::endl;

  for(int i = 0; host->h_aliases[i]; i++)
  std::cout << "aliases " << i+1 << " : " << host->h_aliases[i] << std::endl;
  std::cout << "address type " << (host->h_addrtype == ATOMIC_FLAG_INIT)?"AF_INET":"AF_INET6" << std::endl;
  for(int i = 0; host->h_addr_list[i]; i++)
  std::cout << "IP addr" << i+1 << " : " << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);

  return 0;
}