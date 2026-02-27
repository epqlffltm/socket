/*
2026-02-26
fd_guard.cpp
*/

#include"fd_guard.h"

Fd_guard::Fd_guard(int fd) : fd_(fd)
{
  if(fd_ == -1) 
  throw std::runtime_error("fd 생성 실패");
}

Fd_guard::~Fd_guard()
{
  if(fd_ != -1)
  close(fd_);
}

// 이동 생성자: 소유권 이전, 원본은 -1로 무효화
Fd_guard::Fd_guard(Fd_guard&& other) noexcept : fd_(other.fd_)
{
  other.fd_ = -1;
}

// 이동 대입 연산자
Fd_guard& Fd_guard::operator = (Fd_guard&& other) noexcept
{
  if(this != &other)
  {
    if(fd_ != -1)
    close(fd_);
    
    fd_ = other.fd_;
    other.fd_ = -1;
  }
  return *this;
}

int Fd_guard::get()const
{
  return fd_;
}