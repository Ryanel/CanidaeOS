# Scheduling in Lobo
The Lobo kernel is able to handle and run multiple threads at the same time. Every thread is assigned to a task.

## Task
A task is a collection of resources, including threads, and a virtual address space. 
The Tasks' threads use it's resources to run executable code.

Each task at a minimum has a TaskID, a state, a name, and a VAS.

A task can optionally contain additional resources, such as file handles.

Tasks can communicate with eachother through ports (message queues), or a Shared Memory resource.

### Creation
When a Task is created, it is assigned a VAS, which is a clone of the creating processes' VAS. Alternatively, it can be given a clone of the kernel idle threads VAS, which is empty except for the kernel.

### Kernel Worker Threads
All kernel worker threads are under one task, so they all share the same VAS.

### Thread
A thread is an CPU execution context within a Task. Each thread is created by the kernel.
A thread does not have it's own VAS, instead using it's owning Task's VAS. Each thread has a state, name, threadID, parent taskID, and stack pointer.