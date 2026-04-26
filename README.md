# 🖥️ Mini OS - Phase 1

A minimal operating system shell built entirely from scratch in C, featuring custom-implemented system libraries. This project demonstrates low-level systems programming by bypassing the standard C library for all core logic.

## 🚀 Overview

In Phase 1, we have established the foundational "engine" of the OS. This includes five core libraries for memory management, string manipulation, math, screen rendering, and keyboard interaction. The system boots into a functional interactive shell that parses and executes commands in real-time.

## 🛠️ Core Libraries (The Engine)

Unlike traditional C programs, this project implements its own versions of standard functionality:

| Library | Functionality |
| :--- | :--- |
| **`math.c`** | Multiplication, division, modulo, absolute value, and boundary helpers. Fully supports negative integers. |
| **`string.c`** | Length, copy, compare, tokenization (`str_split`), and signed int/string conversions. |
| **`memory.c`** | Virtual RAM management with a global region and custom `alloc`/`dealloc`. |
| **`screen.c`** | ANSI-powered terminal clearing, cursor positioning, and text rendering. |
| **`keyboard.c`** | Non-blocking input detection (`key_pressed`) and line reading. |
| **`security.c`** | Input sanitization to prevent buffer overflows and invalid memory access. |

## 💻 The Shell

The Mini OS Shell provides a command-line interface to interact with the system.

### Available Commands:
- `help`: Display the command directory.
- `echo <text>`: Print text back to the screen (verifies string/memory integration).
- `add/mul/div/mod <a> <b>`: Perform math logic using the custom math engine (supports negative numbers).
- `status`: Show diagnostic information about the virtual environment.
- `clear`: Reset the terminal view.
- `exit`: Shutdown the virtual OS.

## ⚙️ Building and Running

### Prerequisites:
- GCC compiler
- A Unix-like terminal (MacOS/Linux)

### Build Instructions:
```bash
gcc -I. main.c math/math.c string/string.c memory/memory.c screen/screen.c keyboard/keyboard.c security/security.c -o mini-os
```

### Execution:
```bash
./mini-os
```

## 📜 Compliance with Rules
- ✅ **No Standard C Libraries**: No `<string.h>` or `<math.h>`.
- ✅ **Hardware Abstraction**: `<stdio.h>` is used only for character I/O.
- ✅ **Dynamic Logic**: No hard-coded values; calculations and parsing are dynamic.
- ✅ **Library Pipeline**: Keyboard input → String parsing → Memory management → Math logic → Screen rendering.

---
*Created as part of the Final Project Capstone.*
