#include<iostream>
#include<cstdlib>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<cstring>

void error_handling(char* message);

int main(void)
{
  int fd;
  char buf[]="Let's go!\n";
  fd=open("data.txt", O_CREAT|O_WRONLY|O_APPEND,0644);
  if(fd==-1) {
    std::cerr << "errno: " << errno << std::endl;
    std::cerr << "error: " << strerror(errno) << std::endl;
    error_handling((char*)"open() error");
  }
  std::cout<<"file descriptor: "<<fd<<std::endl;

  if(write(fd,buf,sizeof(buf))==-1)
    error_handling((char*)"write() error");
  close(fd);

  return 0;
}

void error_handling(char* message)
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}