/*
2026-03-02
쓰레드를 메모리에
*/

#include<iostream>
#include<thread>
#include<vector>

void work(int &counter);

int main(void)
{
  int counter = 0;

  std::vector<std::thread>works;

  for(int i = 0; i<4; i++)
  {
    works.push_back(std::thread(work,std::ref(counter)));
  }

  for(int i = 0; i<4; i++)
  {
    works[i].join();
  }

  std::cout << "Counter 최종 값 : " << counter << std::endl;

  return 0;
}

void work(int &counter)
{
  for(int i = 0; i<10000; i++)
  {
    counter += 1;
  }
}