/*
2026-03-03
데드락
*/

#include<iostream>
#include<thread>
#include<mutex>

void work1(std::mutex *m1, std::mutex *m2, int *n);
void work2(std::mutex *m1, std::mutex *m2, int *n);

int main(void)
{
  int n1 = 0, n2 = 0;
  std::mutex m1,m2;
  std::thread t1(work1, &m1,&m2, &n1);
  std::thread t2(work2, &m1,&m2, &n2);



  t1.join();
  t2.join();

  std::cout << "n1 = " << n1 << "| n2 = " << n2 << std::endl;

  return 0;
}

void work1(std::mutex *m1, std::mutex *m2, int *n)
{
  for(int i = 0; i<10; i++)
  {
    while(true)
    {
      std::unique_lock<std::mutex>lock1(*m1);
      std::cout << "work1 -> m1 : " << i << std::endl; 
      if(!m1->try_lock())
      {
        lock1.unlock();
        continue;
      }
      std::lock_guard<std::mutex>lock2(*m2);
      std::cout << "work1 -> m2 : " << i << std::endl; 
      (*n) ++;
      break;
    }
  }
}

void work2(std::mutex *m1, std::mutex *m2, int *n)
{
  for(int i = 0; i<10; i++)
  {
    while(true)
    {
      std::unique_lock<std::mutex>lock2(*m2);
      std::cout << "work2 -> m2 : " << i << std::endl; 
      if(!m1->try_lock())
      {
        lock2.unlock();
        continue;
      }
      std::lock_guard<std::mutex>lock1(*m1);
      std::cout << "work2 -> m1 : " << i << std::endl; 
      (*n) ++;
      break;
    }
  }
}