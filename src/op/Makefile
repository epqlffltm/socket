#makefile

CC = g++
CFLAGS = -Wall -g

SRCG = $(wildcard *.cpp)

TARGET = $(SRCG:.cpp=)

all: $(TARGET)
%: %.cpp
	$(cc) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)