CC = gcc

CFLAGS = -Iinclude

SRCS = src/main.c src/pid.c src/built_in.c src/execute_command.c

TARGET = myshell

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)