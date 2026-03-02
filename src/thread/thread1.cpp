/*
2026-03-02
thread
*/

#include<iostream>
#include<thread>
#include<unistd.h>

void* th(void *arg);

int main(int argc, char *arcv[])
{
  thread.t1(th);
  thread.t2(th);
  thread.t3(th);

  t1.join();
  t2.join();
  t3.join();

  return 0;
}

void* th(void *arg)
{
  int cnt=*((int*)arg);
  for(int i = 0; i < 10; i++)
  {
    std::cout<< sleep(1) << "therad : " << i << std::endl;
  }
}