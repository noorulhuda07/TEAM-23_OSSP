# OSSP 25CS2104E - Week 7 Task 2

## Title
Virtual Memory Mapping Analysis Using `/proc/<PID>/maps`

## Aim
To study the virtual memory organization of a running Linux process using `/proc/<PID>/maps` and the `pmap` command.

## Objectives
1. Study Linux virtual memory organization.
2. Identify code, data, heap and stack regions.
3. Analyze memory mappings of a running process.
4. Understand memory access permissions.
5. Use `/proc/<PID>/maps` and `pmap` for memory analysis.

## Theory

Each Linux process has its own virtual address space. It contains different regions such as:

- Text/Code – contains program instructions.
- Data – contains initialized global and static variables.
- BSS – contains uninitialized global and static variables.
- Heap – used for dynamically allocated memory.
- Stack – stores local variables and function-call information.
- Shared Libraries – libraries such as `libc.so.6`.
- VDSO – kernel-provided virtual shared object.

The `/proc/<PID>/maps` file displays the virtual memory mappings of a running process.

### Memory Permissions

| Permission | Meaning |
|---|---|
| r | Read |
| w | Write |
| x | Execute |
| p | Private mapping |
| s | Shared mapping |

## Program

The program `memory_maps.c` prints the addresses of global, static, heap and stack variables and keeps the process running so that its memory mappings can be inspected.

## Compilation

```bash
gcc -Wall -Wextra -std=c11 memory_maps.c -o memory_maps

