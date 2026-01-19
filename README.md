# myshell 🐚

A simple Unix-like shell written in C for learning system programming.

## Features

- Built-in commands:
  - `cd` → change directory
  - `exit` → exit the shell
- External commands support using `fork` + `execvp`
- Current working directory prompt
- Works with standard Linux commands (`ls`, `pwd`, `cat`, etc.)

## Usage

```bash
./myshell
myShell> ls
myShell> cd include
myShell> pwd
myShell> exit
