CC = gcc

CFLAGS = -Iinclude

SRCS = src/main.c src/pid.c

TARGET = myshell

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)