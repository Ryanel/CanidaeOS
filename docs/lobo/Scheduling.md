# Scheduling in Lobo
The Lobo kernel is able to handle and run multiple threads at the same time. Every thread is assigned to a task.

## Task
A task is a collection of assigned resources and a private virtual address space.
A task can (and usually does) have threads. It will also have a Task ID.

A task can optionally contain additional resources, such as file handles.

Tasks can communicate with eachother through ports (message queues), or a Shared Memory resources.

### Thread
A thread is an CPU execution context within a Task. Each thread is created by the kernel.

### Address Space
Each task has its own private virtual address space (page table). All threads share the same address space.