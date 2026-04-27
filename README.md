# Mini OS

A simulated command-line operating system running entirely inside virtual memory. Built from scratch in C with no standard library dependencies for core logic. Includes a shell, a virtual file system, and cooperative task scheduling.

---

## Project Description

Mini OS boots into an interactive shell and simulates a complete operating environment in user space:

- **Shell** — parses and dispatches commands using a custom `string.c` tokenizer
- **Virtual File System (VFS)** — stores files as allocated blocks inside a 1 MB virtual RAM array managed by `memory.c`
- **Task Scheduler** — runs background tasks concurrently with the shell using cooperative round-robin scheduling via POSIX `ucontext`

---

## Commands

| Command | Description |
|---|---|
| `help` | List all available commands |
| `echo <text>` | Print text back to the screen |
| `add <a> <b> ...` | Add numbers |
| `sub <a> <b> ...` | Subtract numbers |
| `mul <a> <b> ...` | Multiply numbers |
| `div <a> <b> ...` | Divide numbers |
| `mod <a> <b> ...` | Modulo operation |
| `ls` | List all files in the VFS |
| `touch <name> [text]` | Create a new file (with optional initial content) |
| `read <name>` | Print the contents of a file |
| `write <name> <text>` | Append text to an existing file |
| `rm <name>` | Delete a file and free its memory |
| `run counter` | Spawn a background counter task |
| `run ping` | Spawn a background ping task |
| `ps` | List all active processes |
| `kill <pid>` | Terminate a background process |
| `status` | Show memory usage and system info |
| `clear` | Clear the terminal screen |
| `exit` | Shut down the OS |

---

## Build and Run

### Requirements
- GCC
- macOS or Linux (uses POSIX `ucontext` and `termios`)

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
