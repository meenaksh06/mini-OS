# Mini OS - Phase 1

A minimal operating system shell built entirely from scratch in C, featuring custom-implemented system libraries. This project demonstrates low-level systems programming by bypassing the standard C library for all core logic.

## Overview

In Phase 1, we have established the foundational "engine" of the OS. This includes five core libraries for memory management, string manipulation, math, screen rendering, and keyboard interaction. The system boots into a functional interactive shell that parses and executes commands in real-time.

## Core Libraries (The Engine)

Unlike traditional C programs, this project implements its own versions of standard functionality:

| Library | Functionality |
| :--- | :--- |
| **`math.c`** | Multiplication, division, modulo, absolute value, and boundary helpers. Fully supports negative integers. |
| **`string.c`** | Length, copy, compare, tokenization (`str_split`), and signed int/string conversions. |
| **`memory.c`** | Virtual RAM management with a global region and custom `alloc`/`dealloc`. |
| **`screen.c`** | ANSI-powered terminal clearing, cursor positioning, and text rendering. |
| **`keyboard.c`** | Non-blocking input detection (`key_pressed`) and line reading. |
| **`security.c`** | Input sanitization to prevent buffer overflows and invalid memory access. |
| **`vfs.c`** | Virtual File System for in-memory file creation, reading, updating, and deletion. |

## The Shell

The Mini OS Shell provides a command-line interface to interact with the system.

### Available Commands:
- `help`: Display the command directory.
- `echo <text>`: Print text back to the screen (verifies string/memory integration).
- `add/sub/mul/div/mod <a> <b>`: Perform math logic using the custom math engine (supports negative numbers).
- `ls`, `touch`, `cat`, `append`, `rm`: Virtual File System (VFS) operations.
- `status`: Show diagnostic information about the virtual environment.
- `clear`: Reset the terminal view.
- `exit`: Shutdown the virtual OS.

## ⚙️ Building and Running

### Prerequisites:
- GCC compiler
- A Unix-like terminal (MacOS/Linux)

### Build
```bash
gcc -I. main.c math/math.c string/string.c memory/memory.c \
    screen/screen.c keyboard/keyboard.c security/security.c \
    vfs/vfs.c process/process.c -o mini-os
```

### Run
```bash
./mini-os
```

---

## Development Phases

### Phase 1 — Core Shell & Memory Setup
Custom libraries built: `math.c`, `string.c`, `memory.c`, `screen.c`, `keyboard.c`, `security.c`. Shell boots, prints a prompt, parses commands using `string.c`, and tests basic `alloc` through string and math operations.

### Phase 2 — Complete OS Features
Full simulation complete. The Virtual File System (`vfs.c`) stores and retrieves files in virtual RAM. Process scheduling (`process.c`) runs background tasks without freezing the shell — the prompt remains responsive while tasks execute cooperatively between keystrokes.

---

## Source Layout

```
mini-OS/
├── main.c               Shell entry point + background task demos
├── math/                Custom arithmetic library
├── string/              String operations (tokenizer, int conversion)
├── memory/              1 MB virtual RAM, bump allocator with free list
├── screen/              ANSI terminal output
├── keyboard/            Blocking and non-blocking input (read_line_bg)
├── security/            Input sanitisation, buffer-overflow guard
├── vfs/                 Virtual File System (in-memory, up to 10 files)
└── process/             PCB, process table, cooperative round-robin scheduler
```

---

## Known Issues

- **Background task output interleaves with the prompt.** When a background task prints while the user is typing, the output appears mid-line. There is no double-buffering or output queue — this is a known limitation of cooperative terminal-based scheduling.
- **No process isolation.** All processes share the same address space and virtual RAM. A buggy task could overwrite another task's memory.
- **Bump allocator does not compact.** Freed blocks can be reused via first-fit, but fragmentation is not resolved — heavy create/delete cycles will eventually exhaust the allocator even if logical usage is low.
- **Maximum 10 files** in the VFS and **16 concurrent processes** due to fixed-size static tables.
- **`ucontext` is deprecated on macOS** (since 10.6) but still functional. The `_XOPEN_SOURCE 700` define is required to unlock the API.
