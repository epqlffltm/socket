#makefile

CC = g++

CFLAGS = -wall -g

SRCG = $(wildcard *.cpp)

TARGET = $(SRG:.cpp=)

all: $(TARGET)
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)