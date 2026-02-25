#makefile

CC = g++
CFLAGS = -Wall -g

SRCG = $(wildcard *.cpp=)

TARGET = $(SRCG:.app=)

all: $(TARGET)
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)