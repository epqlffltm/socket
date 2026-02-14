#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sys/socket.h>

int main(void)
{
  int fd1, fd2,fd3;
  fd1=socket(PF_INET, SOCK_STREAM,0);//TCP
  fd2=open("text.txt", O_CREAT|O_WRONLY|O_APPEND);//file open
  fd3=socket(PF_INET, SOCK_DGRAM,0);//UDP

  std::cout<<"fd1: "<<fd1<<std::endl;
  std::cout<<"fd2: "<<fd2<<std::endl;
  std::cout<<"fd3: "<<fd3<<std::endl;

  close(fd1);
  close(fd2);
  close(fd3);

  return 0;
}