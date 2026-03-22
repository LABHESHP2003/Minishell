# Minishell - A Custom Unix Shell Implementation

## Overview

Minishell is a minimalistic Unix shell implementation that provides essential shell functionalities including command execution, job control, signal handling, and pipeline support. This project demonstrates understanding of process management, system calls, and Unix internals.

## Features

### Core Features
- **Command Execution**
  - Execute external commands via `fork()` and `execvp()`
  - Built-in commands for shell operations
  - Proper error handling for command not found, permission denied, etc.

- **Customizable Prompt**
  - Default prompt: `minishell$ `
  - Change prompt using `PS1=NEW_PROMPT` (no spaces allowed around `=`)

- **Special Variables**
  - `echo $$` - Display shell's PID
  - `echo $?` - Display last command's exit status
  - `echo $SHELL` - Display shell executable path

### Built-in Commands

| Command | Description |
|---------|-------------|
| `cd [dir]` | Change current working directory |
| `pwd` | Print current working directory |
| `exit` | Exit the shell |
| `clear` | Clear the terminal screen |
| `jobs` | List background and stopped jobs |
| `fg [pid]` | Bring job to foreground |
| `bg` | Resume stopped job in background |

### Job Control
- Background execution support
- Process management with linked list
- Job listing with `jobs` command
- Foreground/background process switching

### Signal Handling
- **Ctrl+C (SIGINT)**: Interrupt foreground process
- **Ctrl+Z (SIGTSTP)**: Stop foreground process

### Pipeline Support
- Execute multiple commands connected with pipes `|`
- Examples:
  - `ls -l | wc -l`
  - `ps aux | grep bash | wc -l`

## Getting Started

### Prerequisites
- Linux/Unix OS
- GCC compiler
- Make utility

### Installation

```bash
git clone https://github.com/LABHESHP2003/Minishell.git
cd Minishell
make
./minishell
```

### Build Options

| Command | Description |
|--------|-------------|
| `make` | Build executable |
| `make clean` | Remove object files |
| `make cleanall` | Remove all |
| `make run` | Build and run |

## Usage Examples

### Basic Commands

```bash
minishell$ pwd
/home/user/minishell

minishell$ ls -la
total 128
drwxr-xr-x  3 user user  4096 Mar 22 10:30 .
drwxr-xr-x  5 user user  4096 Mar 22 10:30 ..
-rw-r--r--  1 user user  1234 Mar 22 10:30 main.c
...

minishell$ cd /tmp
minishell$ pwd
/tmp
```

### Special Variables

```bash
minishell$ echo $$
12345
minishell$ echo $?
0
minishell$ echo $SHELL
/home/user/minishell/minishell
```

### Custom Prompt

```bash
minishell$ PS1=my_shell> 
my_shell> pwd
/home/user
my_shell> PS1=minishell$ 
minishell$ 
```

### Pipeline Examples

```bash
minishell$ ls -l | wc -l
12

minishell$ ps aux | grep bash | wc -l
3

minishell$ cat /etc/passwd | grep /bin/bash | cut -d: -f1
root
user
```

### Job Control

```bash
minishell$ sleep 100
^Z
Child 12346 stopped by signal 20
[12346] Stopped

minishell$ jobs
[12346] Stopped

minishell$ bg
[12346] Running

minishell$ jobs
[12346] Running

minishell$ fg
sleep 100
^C
Child 12346 terminated by signal 2
```

### Exit Status

```bash
minishell$ ls /nonexistent
ls: cannot access '/nonexistent': No such file or directory
minishell$ echo $?
2

minishell$ exit
```

## Project Structure

```
Minishell/
├── main.c                          # Main program entry point
├── main.h                          # Header file with all declarations
├── common.c                        # Utility functions (parsing, type detection)
├── execute_external_commands.c     # External command execution
├── execute_internal_commands.c     # Built-in command execution
├── job_management.c                # Background job management
├── pipe_handling.c                 # Pipe command execution
├── signal_handling.c               # Signal handlers (SIGINT, SIGTSTP)
├── Makefile                        # Build configuration
└── README.md                       # This file
```

## Technical Details

### System Calls Used

* `fork()` - Create child processes
* `execvp()` - Execute external programs
* `waitpid()` - Wait for process state changes
* `pipe()` - Create communication channels
* `dup2()` - Redirect file descriptors
* `chdir()` - Change working directory
* `getcwd()` - Get current working directory
* `kill()` - Send signals to processes
* `sigaction()` - Set signal handlers
* `strtok()` - Parse input strings

### Process Management

* Parent process tracks foreground PID
* Background jobs stored in linked list
* Process states: RUNNING, STOPPED
* Automatic job cleanup on termination

### Signal Handling Strategy

* **SIGINT**: Forward to foreground process or redisplay prompt
* **SIGTSTP**: Stop foreground process and add to job list
* **SA_RESTART**: Automatically restart interrupted system calls

## Learning Outcomes

This project demonstrates understanding of:

1. Process Creation and Management: fork(), exec(), wait()
2. Inter-process Communication: pipes for command chaining
3. Signal Handling: custom handlers for Ctrl+C, Ctrl+Z
4. Job Control: background processes, job listing
5. File Descriptor Management: stdin/stdout redirection
6. String Parsing: command line tokenization
7. Error Handling: proper error messages and exit codes

## Requirements Met

| Requirement                        | Status     |
| ---------------------------------- | ---------- |
| Customizable prompt (PS1)          | ✅ Complete |
| External command execution         | ✅ Complete |
| Special variables ($$, $?, $SHELL) | ✅ Complete |
| Signal handling (Ctrl+C, Ctrl+Z)   | ✅ Complete |
| Built-in commands (cd, pwd, exit)  | ✅ Complete |
| Job control (jobs, fg, bg)         | ✅ Complete |
| Pipe functionality                 | ✅ Complete |

## Author

**Labhesh Patil**

* GitHub: [@LABHESHP2003](https://github.com/LABHESHP2003)

---

## Quick Reference

### Basic Commands

```bash
pwd              # Show current directory
cd [dir]         # Change directory
exit             # Exit shell
clear            # Clear screen
jobs             # List background jobs
fg [pid]         # Bring job to foreground
bg               # Resume stopped job
```

### Special Variables

```bash
echo $$          # Show shell PID
echo $?          # Show last exit status
echo $SHELL      # Show shell path
```

### Pipeline Examples

```bash
cmd1 | cmd2                    # Single pipe
cmd1 | cmd2 | cmd3              # Multiple pipes
ls -l | grep ".c" | wc -l       # Count C files
```

### Signal Shortcuts

```
Ctrl+C     # Send SIGINT to foreground process
Ctrl+Z     # Send SIGTSTP to stop foreground process
```

---

**Built with ❤️ for learning Linux internals**
```

This README is:
- Clean and properly formatted
- Uses consistent markdown syntax
- Has all sections properly structured
- Shows actual functionality that works in your code
- Removed the "to be implemented" notes since your code already has job control working
- Simplified the content while keeping all important information
