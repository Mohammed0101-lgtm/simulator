# Simulated Kernel and CPU Project

This repository is dedicated to the development of a simulated kernel and CPU that can execute user-written code. The goal of this project is to create a virtual environment where low-level system operations, such as memory management, context switching, and basic CPU instructions, are simulated.

Currently, this project is under active development and focuses on the following key components:

- **Simulated CPU**: Emulates the behavior of a simple CPU, executing instructions such as `LOAD`, `STORE`, `MOV`, `ADD`, `SUB`, `JMP`, and `SYSCALL`.
- **Memory Management**: Simulates a memory management system that handles virtual memory, page tables, and physical memory allocation.
- **Context Switching and Scheduling**: Implements basic process scheduling and context switching to simulate multitasking between different processes.
- **Syscalls**: Handles system calls (such as printing to the console or halting the process), allowing for interaction with the simulated environment.
- **User Code Execution**: The kernel is designed to eventually load and execute real compiled code (ELF binaries), providing a testing ground for low-level code execution.

## Features Under Development

This project is still under development. While the basic architecture is in place, several key features are still being worked on, such as:

- **Complete CPU Instruction Set**: Expanding the available instructions for the CPU to handle more complex operations.
- **File System Support**: Developing support for file I/O and handling more advanced syscalls.
- **User Code Execution**: The ability to load and interpret real ELF binaries is still under development.
- **Exception Handling**: Improvements to handle various exceptions and errors during execution.

## Install

To use or contribute to this project, follow the steps below:

1. Clone the repository:

   ```bash
   git clone https://github.com/Mohammed0101-lgtm/simulator
   cd simulator


### Note
This project is still under active development. Some features might be incomplete or unstable, and some components may change as development progresses.

### Explanation:
- The **Introduction** section gives a brief overview of the project and what it's trying to achieve.
- The **Features Under Development** section highlights that the project is ongoing and what is still being worked on.
- **Installation** describes how to set up the project on your machine.
- **Contributing** encourages contributions from other developers who may want to help expand the project.
- **License** mentions the license type for the repository.

You can modify the instructions to fit your specific setup, such as adjusting the build commands or adding additional setup steps.
