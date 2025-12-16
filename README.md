# TP_Prog_Sys_ENSEA_Shell_OUERFELLI_CAPODAGLI

![EnseaSH Screenshot](image.png)

## ENSEA Shell (enseash)

A lightweight, custom command-line interpreter developed in **C** as part of the **System Programming** course at **ENSEA**.

This project implements a functional shell capable of executing system commands, measuring execution time, and handling complex I/O operations like redirections and pipes, while strictly adhering to low-level system call constraints.

** Authors:**
* **Capodagli Janus**
* **Ouerfelli Karim**


---

## Project Philosophy & Constraints

We prioritized **Code Quality** and **Architecture** over feature quantity. This project strictly follows these guidelines:

1.  **Strictly "No Printf":**
    * The standard `printf` family is forbidden to avoid buffering issues with system calls.
    * **Solution:** We implemented custom output helpers (`display_int`, `write_out`) using `write()` directly on file descriptors.
2.  **No Magic Numbers:**
    * Hardcoded values are replaced by explicit constants in `enseash.h` (e.g., `NS_PER_MS`, `FD_STDOUT`, `FILE_PERMS`).
3.  **Modular Architecture:**
    * The code is split into logical units: **Main Loop** (Orchestration) and **Shell Logic** (Parsing/Execution).
4.  **English Only:**
    * All variable names, comments, and documentation are written in English.

---

## Features Implemented (Q1 - Q8)

The shell supports the following functionalities:

* **Prompt Display:** Shows the exit status (`exit:0` or `exit:1`) and execution time of the previous command.
* **Command Execution:** Runs standard Linux commands (e.g., `ls`, `date`, `uname`) with arguments.
* **Time Measurement:** Measures execution time using `clock_gettime` (displayed in milliseconds).
* **Signal Handling:** Detects if a command was killed by a signal and displays it.
* **Redirections:**
    * **Output (`>`):** Redirects standard output to a file (e.g., `ls > log.txt`).
    * **Input (`<`):** Redirects standard input from a file (e.g., `wc -l < log.txt`).
* **Pipes (`|`):** Connects the output of one process to the input of another (e.g., `ls | wc -l`).

---

## Project Structure

The project follows a clean separation of concerns:

```

.
├── Core
│   ├── Inc
│   │   └── enseaShell.h      \# Constants, Prototypes, Macros
│   └── Src
│       ├── enseaShell.c      \# Core logic (Parsing, Fork, Exec, Pipe, Redirections)
│       └── main.c         \# Main loop (UI, Timing, Signal handling)
├── Makefile               \# Build script
├── README.md              \# Project documentation
└── image.png              \# Screenshot

````

---

## Installation & Usage

### Prerequisites
* Linux Environment (or WSL)
* GCC Compiler
* Make

### Compilation
Use the provided Makefile to compile the project:

```bash
make
````

### Running the Shell

Start the shell:

```bash
./enseaShell
```

### Examples

**1. Simple execution with timing:**

```bash
enseash % sleep 2
enseash [exit:0|2002ms] %
```

**2. Redirection:**

```bash
enseash % ls -l > output.txt
enseash [exit:0|0ms] %
```

**3. Piping:**

```bash
enseash % ls | grep .c | wc -l
2
enseash [exit:0|2ms] %
```

**4. Exiting:**
Type `exit` or press `Ctrl+D`.

-----


<!-- end list -->

```
```