#makefile

CC = g++
CFLAGS = -Wall -g -pthread

SRCG = $(wildcard *.cpp)

TARGET = $(SRCG:.cpp=)

all:$(TARGET)
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)