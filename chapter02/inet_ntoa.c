#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in addr1, addr2;
  char *str_ptr;
  char str_ary[20];

  addr1.sin_addr.s_addr=htonl(0x1020304);
  addr2.sin_addr.s_addr=htonl(0x1010101);

  str_ptr=inet_ntoa(addr1.sin_addr);
  strcpy(str_ary, str_ptr);
  printf("Dotted-Decilml notation1: %s\n",str_ptr);

  inet_ntoa(addr2.sin_addr);
  printf("Dotted-Decilml notation2: %s\n",str_ptr);
  printf("Dotted-Decilml notation3: %s\n",str_ptr);

  return 0;
}