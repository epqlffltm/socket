/*
2026-02-15
파일 생성 밎 데이터의 저장
*/

#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<fcntl.h>
#include"error_handling.h"

int main(void)
{
  //int fd = -1;
  char buf[] = "let's go!\n";
  int flags = O_CREAT|O_RDWR|O_APPEND;
  int fd = open("data.txt", flags);
  if(fd == 1)
  {
    error_handling("open");
  }

  std::cout << "file descriptor" << std::endl;

  if(write(fd, buf, sizeof(buf)) == -1)
  {
    error_handling("write");
  }

  close(fd);

  return 0;
}
