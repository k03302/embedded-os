# Introduction
A simple RTOS built from scratch by following the book "Embedded OS Development Project"

[>>book link](https://product.kyobobook.co.kr/detail/S000001033087)

[>>author's github link](https://github.com/navilera/Navilos)

# Chapter Progress

| Chapter | Topic | Status | Branch/Tag | Description |
|---------|-------|--------|------------|-------------|
| 3 | Tutorial | ✅ Complete | [ch3](https://github.com/k03302/embedded-os/tree/ch3) | Basic ARM assembly, linker script setup |
| 4 | Booting | ✅ Complete | [ch4](https://github.com/k03302/embedded-os/tree/ch4) | Boot sequence, vector table, entry point |
| 5 | UART | ✅ Complete | [ch5](https://github.com/k03302/embedded-os/tree/ch5) | Serial communication, console output |
| 6 | Interrupt | ✅ Complete | [ch6](https://github.com/k03302/embedded-os/tree/ch6) | Exception vectors, interrupt handlers |
| 7 | Timer | 🚧 In Progress | - | System timer, time management |
| 8 | Task | 📋 Planned | - | Task control blocks, task creation |
| 9 | Scheduler | 📋 Planned | - | Round-robin, priority scheduling |
| 10 | Context switching | 📋 Planned | - | Register saving, task switching |
| 11 | Event | 📋 Planned | - | Event flags, synchronization primitives |
| 12 | Messaging | 📋 Planned | - | Message queues, IPC mechanisms |
| 13 | Synchronizing | 📋 Planned | - | Mutex, semaphore, critical sections |


# Build Instructions

| Command | Description |
|---------|-------------|
| `make` | Build the project |
| `make clean` | Clean build artifacts |
| `make run` | Run in QEMU |
| `make debug` | Run with GDB debugging |
| `make gdb` | Launch GDB |

# Hardware Specifications

| Component | Specification |
|-----------|---------------|
| **Architecture** | ARMv7-A |
| **CPU** | Cortex-A8 |
| **Board** | RealView Platform Baseboard |
| **Endianness** | Little Endian |
| **Memory** | 256MB RAM |