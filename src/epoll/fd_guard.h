/*
2026-02-26
fd_guard.h
*/

#pragma once
#include<stdexcept>
#include<unistd.h>

class Fd_guard
{
  public:
  explicit Fd_guard(int fd);// fd 받아서 유효성 검사, -1이면 즉시 예외

  ~Fd_guard();// 객체 소멸 시 자동 close

  // 복사 금지: 같은 fd를 두 번 close하는 버그 방지
  Fd_guard(const Fd_guard&) = delete;
  Fd_guard& operator = (const Fd_guard&) = delete;

  // 이동 허용: FdGuard를 vector 등에 담을 수 있게
  Fd_guard(Fd_guard&& other) noexcept;
  Fd_guard& operator = (Fd_guard&& other) noexcept;

  int get() const;//fd 값 반환

  private:
  int fd_;
};