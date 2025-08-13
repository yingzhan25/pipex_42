# pipex_42

A Unix pipe emulator written in C, developed as part of the 42 School curriculum.

---

## 📚 Project Overview

**pipex_42** replicates the shell's pipe `|` functionality, allowing you to execute a sequence of commands where the output of one command serves as the input for the next. This project demonstrates a deep understanding of process management, file descriptors, and inter-process communication in a Unix environment.

---

## 🚀 Features

- Executes multiple commands connected via pipes
- Handles input and output redirection
- Mimics shell behavior for command pipelines
- Robust error handling and resource management

---

## 🛠️ Getting Started

### Prerequisites

- GCC or any C compiler
- Make

### Build

```bash
make
```

### Usage

```bash
./pipex file1 cmd1 cmd2 file2
```

- `file1`: Input file
- `cmd1`: First command to execute
- `cmd2`: Second command to execute
- `file2`: Output file

Example:

```bash
./pipex infile "grep hello" "wc -l" outfile
```

This will execute:  
`< infile grep hello | wc -l > outfile`

---

## 📁 Project Structure

- `src/` – Source code files
- `include/` – Header files
- `Makefile` – Build instructions

---

## 📖 Learning Objectives

- Master file descriptors and pipes in Unix
- Understand process forking and exec family functions
- Develop defensive C programming skills

---

## 👤 Author

- [Yingzhan](https://github.com/yingzhan25)

---

> Developed as a part of the 42 School curriculum.
