/*
2026-03-02
thread2
*/

#include<iostream>
#include<thread>
#include<vector>

void th(const std::vector<int>& data, int start, int end, int* num);

int main(int argc, char *argv[])
{
  const int n = 10000;
  int chunk = n / 4;  // 2500
  int total = 0;

  std::vector<int> data;
  for(int i = 1; i <= n; i++)
  {
    data.push_back(i);
  }

  std::vector<int> result(4, 0);      // 각 스레드 결과 저장
  std::vector<std::thread> threads;   // 스레드 저장

  for(int i = 0; i < 4; i++)
  {
    int start = i * chunk;
    int end = (i + 1) * chunk;
    threads.push_back(std::thread(th, std::ref(data), start, end, &result[i]));
  }

  for(int i = 0; i < 4; i++)
  {
    threads[i].join();
    total += result[i];
  }

  std::cout << "전체 합 : " << total << std::endl;

  return 0;
}

void th(const std::vector<int>& data, int start, int end, int* num)
{
  for(int i = start; i < end; i++)
    {
      *num += data[i];  // data 배열에서 값 가져옴
    }
}