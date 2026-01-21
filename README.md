# myshell

A minimal shell implementation in C that demonstrates core Unix process management
and command execution fundamentals. This project separates concerns into discrete
modules for command parsing, built-in operations, and external process spawning.

## Architecture

The shell is structured around three primary execution paths:

- **built_in.c**: Handles built-in commands (cd, pwd, exit, help) without process
  forking. These operations execute within the parent shell context.

- **pid.c**: Manages external command execution through fork/execvp. Returns exit
  status to enable proper command chaining behavior. Error handling differentiates
  between fork failures and command not found conditions.

- **execute_command.c**: Route dispatcher that checks for built-in commands first,
  then delegates to pid_function for external commands. Centralizes control flow
  logic and status propagation.

- **main.c**: Event loop that tokenizes input, detects logical AND operators (&&),
  and chains command execution based on success/failure semantics. Manages prompt
  rendering and interactive state.

## Built-in Commands

- cd [path]     Change working directory (no args: go to HOME)
- pwd           Print current working directory
- exit          Terminate shell
- help          Display command reference

## Operator Support

- &&             Logical AND - execute right side only if left side succeeds
                 Example: cd /tmp && ls

## Building

  make

This compiles all source modules with include path set to the include directory.

## Running

  ./myshell

The shell displays the current working directory in the prompt and continues
reading commands until exit is invoked.

## External Commands

The shell forks a child process and executes external commands via execvp().
The parent process waits for completion and propagates exit codes up through
the call stack. This enables proper && operator semantics.

## Notes

This implementation prioritizes clarity over performance. It is intended as an
educational reference for understanding shell mechanics, not production use.
