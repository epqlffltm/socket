#makefile

CXX = g++

#컴파일 옵션 (표준 설정 및 경고 강화)
CXXFLAGS = -Wall -Wextra -g -pthread -std=c++20

#소스 및 타겟 설정
SRCS = $(wildcard *.cpp)
TARGETS = $(SRCS:.cpp=)

#  가짜 타겟(Phony Targets) 설정
# 파일 이름과 타겟 이름이 겹칠 때 발생하는 오류를 방지
.PHONY: all clean

# 기본 타겟: 모든 실행 파일 생성
all: $(TARGETS)

# 패턴 규칙: %.cpp 파일을 % 실행 파일로 빌드
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# 정리 타겟
clean:
	rm -f $(TARGETS)