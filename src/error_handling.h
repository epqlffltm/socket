/*
error_handling.h
소켓 에러 처리 유틸리티
*/

#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>

inline void error_handling(const char *message)
{
    std::cerr << message << " error: " << strerror(errno) << std::endl;
    exit(1);
}

#endif  // ERROR_HANDLING_H