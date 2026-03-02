/*
2026-03-03
상호 배제 자동 해제
*/

#include<iostream>
#include<vector>
#include<thread>
#include<mutex>

void work(int *reault, std::mutex *m);

int main(void)
{
  int count = 0;
  std::vector<std::thread>v;
  std::mutex m;

  for(int i = 0; i<4; i++)
  {
    v.push_back(std::thread(work, &count, &m));
  }

  for(int i = 0; i < 4; i++)
  {
    v[i].join();
  }
  std::cout << "Counter 최종 값 : " << count << std::endl;

  return 0;
}

void work(int *reault, std::mutex *m)
{
  for(int i = 0; i<10;i++)
  {
    std::lock_guard<std::mutex>lock(m);
    reault += i;
  }
}