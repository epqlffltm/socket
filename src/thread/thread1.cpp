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
  std::thread t1(th,1);
  std::thread t2(th,2);
  std::thread t3(th,3);

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
    std::cout<< "therad : " << id << i << std::endl;
  }
}