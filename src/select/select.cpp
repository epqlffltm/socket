/*
2026-03-25
select server
*/

#include<iostream>
#include<vector>
#include<unistd.h>
#include<sys/time.h>
#include<sys/select.h>

int main([[maybe_unused]]int argc, [[maybe_unused]]char *argv[])
{
  const int kBufSize = 30;

  fd_set reads, temps;
  int result, str_len;
  std::vector <char> buf (kBufSize + 1);//+1은 널 문자(\0)를 위한 공간
  struct timeval timeout;

  FD_ZERO(&reads);
  FD_SET(0, &reads);// 0번은 표준 입력(stdin)

  /*
  timeout.tv_set=5;
  timeout.tv_usec=5000;
  */

  while(true)
  {
    temps = reads;// select 함수는 실행 후 fd_set을 변경하므로 원본(reads)을 복사해서 사용

    // 타임아웃 설정 (매번 다시 초기화해야 함)
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    result = select(1, &temps, nullptr, nullptr, &timeout);

    if(result == -1)
    {
      std::cerr << "select() error" << std::endl;
      break;
    }
    else if(result == 0)
    {
      std::cout << "time-out!" << std::endl;
      break;
    }
    else
    {
      if(FD_ISSET(0, &temps))// 표준 입력(0번)에 데이터가 들어왔는지 확인
      {
        str_len = read(0, buf.data(), kBufSize);
        buf[str_len] = '\0';
        std::cout << "message : " << buf.data() << std::endl;
      }
    }
  }

  return 0;
}
