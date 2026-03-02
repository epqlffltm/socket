/*
2026-03-02
thread
*/

#include<iostream>
#include<thread>
#include<unistd.h>

void th(int id);

int main(int argc, char *arcv[])
{
  std::thread t1(th);
  std::thread t2(th);
  std::thread t3(th);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}

void th(int id)
{
  for(int i = 0; i < 10; i++)
  {
    sleep(1);
    std::cout<< "therad : " << i << std::endl;
  }
}