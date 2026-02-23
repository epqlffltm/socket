#makefile

CC = g++

CFLAGS = -wall -g

SRCG = $(wildcard *.cpp)

TARGET = $(SRCG:.cpp=)

all: $(TARGET)
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)