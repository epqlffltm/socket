#include<iostream>
#include<cstdlib>
#include<fcntl.h>
#include<unistd.h>
#define BUF_SIZE 100
void error_handling(char* message);

int main(void)
{
  int fd=0;
  char buf[BUF_SIZE];

  fd=open("data.txt", O_RDONLY);
  if(fd==-1)
    error_handling((char*)"open() error");
  std::cout<<"file descriptor: "<<fd<<std::endl;

  if(read(fd, buf, sizeof(buf)-1)==-1)
    error_handling((char*)"read() error");
  std::cout<<buf<<std::endl;

  close(fd);
  return 0;
}

void error_handling(char* message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}