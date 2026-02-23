#makefile

CC = g++
CFLAGS = -Wall -g

SRCG = $(wildcard *.cpp)

TARGETG = $(SECG:.cpp=)

all: $(TARGET)
%: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)