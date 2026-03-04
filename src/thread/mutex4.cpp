/*
2026-03-03
스크랩
*/

#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<thread>
#include<mutex>
#include<chrono>

void producer(std::queue<std::string> *dawnloaded_paged, std::mutex *m, int index);
void consumer(std::queue<std::string>* dawnloaded_paged, std::mutex *m, int index);


int main(void)
{
  std::queue<std::string>dawnloaded_paged;
  std::mutex m;

  std::vector<std::thread>producers;
  for(int i = 0; i< 5; i++)
  {
    producers.push_back(std::thread(producer,&dawnloaded_paged, &m, i + 1));
  }

  int num_processed = 0;
  std::vector<std::thread> consumers;
  for (int i = 0; i < 3; i++) 
  {
    consumers.push_back(std::thread(consumer, &dawnloaded_paged, &m, &num_processed));
  }

  for (int i = 0; i < 5; i++) 
  {
    producers[i].join();
  }
  for (int i = 0; i < 3; i++) 
  {
    consumers[i].join();
  }
}

void producer(std::queue<std::string>* dawnloaded_paged, std::mutex* m, int index)
{
  for (int i = 0; i < 5; i++) 
  {
    // 웹사이트를 다운로드 하는데 걸리는 시간이라 생각하면 된다.
    // 각 쓰레드 별로 다운로드 하는데 걸리는 시간이 다르다.
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * index));
    std::string content = "웹사이트 : " + std::to_string(i) + " from thread(" +std::to_string(index) + ")\n";

    // data 는 쓰레드 사이에서 공유되므로 critical section 에 넣어야 한다.
    m->lock();
    dawnloaded_paged->push(content);
    m->unlock();
  }
}

void consumer(std::queue<std::string>* dawnloaded_paged, std::mutex* m, int* num_processed) 
{
  // 전체 처리하는 페이지 개수가 5 * 5 = 25 개.
  while (*num_processed < 25) 
  {
    m->lock();
    // 만일 현재 다운로드한 페이지가 없다면 다시 대기.
    if (dawnloaded_paged->empty()) 
    {
      m->unlock();  // (Quiz) 여기서 unlock 을 안한다면 어떻게 될까요?

      // 10 밀리초 뒤에 다시 확인한다.
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    // 맨 앞의 페이지를 읽고 대기 목록에서 제거한다.
    std::string content = dawnloaded_paged->front();
    dawnloaded_paged->pop();

    (*num_processed)++;
    m->unlock();

    // content 를 처리한다.
    std::cout << content;
    std::this_thread::sleep_for(std::chrono::milliseconds(80));
  }
}