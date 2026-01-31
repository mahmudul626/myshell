CC = gcc

CFLAGS = -Iinclude

SRCS = src/main.c src/pid_function.c src/built_in.c src/execute_command.c src/read_input.c src/auto_complete.c

TARGET = myshell

all:
		$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
		rm -f $(TARGET)