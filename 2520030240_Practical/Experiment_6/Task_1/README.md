# Experiment 6 - Task 1
## Client-Server Application Using Named Pipes (FIFOs)

### Aim

To create a client-server application using Named Pipes (FIFOs), where the client sends messages to the server and the server processes and responds to the client. The behavior of FIFOs when multiple clients communicate with the server is also analyzed.

---

## Theory

A Named Pipe, or FIFO (First-In-First-Out), is an inter-process communication mechanism provided by Linux.

Unlike an ordinary pipe, a named pipe has a name in the filesystem and can be accessed by unrelated processes.

The `mkfifo()` system call is used to create a FIFO.

### Important FIFO Properties

1. FIFO provides communication between processes.
2. Data is read in First-In-First-Out order.
3. A FIFO appears as a special file in the filesystem.
4. The `p` character in `ls -l` output indicates a FIFO.
5. A writer may block until a reader opens the FIFO.
6. Multiple clients can write requests to a common server FIFO.
7. Small writes within the system's atomic pipe-write limit can be kept together.
8. A shared response FIFO can cause clients to receive another client's response.
9. In this experiment, each client creates a unique response FIFO using its process ID.

---

## Architecture

```text
                 Client 1
                    |
                    |
                 Client 2
                    |
                    |  Requests
                    v
        /tmp/ossp_server_fifo
                    |
                    v
                 Server
              /          \
             /            \
            v              v
 Client 1 Response     Client 2 Response
      FIFO                  FIFO

