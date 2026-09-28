# Experiment 5 – Task 2

## Process Communication Using Pipe and Exec

### Aim

To develop a C program that executes the equivalent of the shell command:

```bash
ls -l | grep ".c"
```

using the `fork()`, `pipe()`, `dup2()`, and `exec()` system calls.

### Theory

A UNIX/Linux pipeline connects the output of one command to the input of another command.

The command:

```bash
ls -l | grep ".c"
```

works as follows:

* `ls -l` lists files and their details.
* Its standard output is sent through a pipe.
* `grep ".c"` reads the data from the pipe through its standard input.
* Only lines matching `.c` are displayed.

The program implements the same mechanism using system calls.

### System Calls Used

| System Call | Purpose                                           |
| ----------- | ------------------------------------------------- |
| `pipe()`    | Creates a communication channel between processes |
| `fork()`    | Creates child processes                           |
| `dup2()`    | Redirects standard input/output to the pipe       |
| `execlp()`  | Executes `ls` and `grep`                          |
| `waitpid()` | Waits for child processes to finish               |
| `close()`   | Closes unused pipe ends                           |

### Process Flow

```text
                 Parent Process
                       |
                  pipe(pipefd)
                       |
              +--------+--------+
              |                 |
           fork()            fork()
              |                 |
          Child 1            Child 2
              |                 |
           ls -l             grep ".c"
              |                 ^
              |                 |
       stdout → pipe → stdin
              |
         Parent waits
              |
       Pipeline completed
```

### Algorithm

1. Create a pipe using `pipe()`.
2. Create the first child process using `fork()`.
3. In the first child:

   * Redirect standard output to the pipe's write end using `dup2()`.
   * Close unused pipe descriptors.
   * Execute `ls -l` using `execlp()`.
4. Create the second child process using `fork()`.
5. In the second child:

   * Redirect standard input from the pipe's read end using `dup2()`.
   * Close unused pipe descriptors.
   * Execute `grep ".c"` using `execlp()`.
6. The parent closes both pipe ends.
7. The parent waits for both child processes using `waitpid()`.
8. Display a message indicating that pipeline execution is complete.

### Program

The program is implemented in `pipeline.c`.

### Compilation

```bash
gcc -Wall -Wextra -std=c11 pipeline.c -o pipeline
```

### Execution

```bash
./pipeline
```

### Sample Output

```text
-rwxrwxrwx 1 noor_ul_huda noor_ul_huda 1783 Sep 28 04:12 pipeline.c

Parent: Pipeline execution completed.
```

### Observation

The program successfully creates two child processes. The first child executes `ls -l` and redirects its output to the pipe. The second child receives the pipe data through standard input and executes `grep ".c"`.

Thus, the program reproduces the behavior of:

```bash
ls -l | grep ".c"
```

using process creation, inter-process communication, file-descriptor redirection, and program execution system calls.

### Conclusion

The program successfully demonstrates UNIX/Linux pipeline implementation using `fork()`, `pipe()`, `dup2()`, and `exec()` system calls. It shows how multiple processes can communicate through a pipe to implement shell-like command execution.

