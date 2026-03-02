/*
2026-03-03
상호 배제
*/

#include<iostream>
#include<vector>
#include<thread>
#include<mutex>

void work(int *result, std::mutex *m);

int main(void)
{
  int count = 0;
  std::vector<int>v;
  std::mutex m;

  for(int i = 0; i<10; i++)
  {
    v.push_back(std::tread(work,&count,&m));
  }

  for(int i = 0; i <10; i++)
  {
    v[i].join();
  }
}

void work(int *result, std::mutex *m)
{
  for(int i = 0; i<10000; i++)
  {
    m->lock();
    *result += 1;
    m->unlock();
  }
}