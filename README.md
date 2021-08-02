# CPU-Scheduling-Simulator-in-C
A CPU uses various scheduling algorithms to allocate CPU cycles to every program. This is a C/C++ implementation of those algorithms according to the 5 state model. It makes use of Linux system calls such as fork, exec, pipes (named/unnamed pipes
depending on the requirement of communication), dup/dup2 as well as pthread library.

Let’s consider there are n number of processes named as Proc = 0, 1, …. , n. Each process 
will be in new state at first and then move through different states of given model.
For this scheduling task, the 5 state process model is simulated with the following states: new, ready, running, blocked and exit.

## Fork & EXEC Implementation
- Each of the process states (e.g. New, Ready, Running etc.) will serve as an individual 
process in the simulator program. 

- Each process will be created using fork system call and then call exec for each state 
implementation. Before calling exec each should create pipes where required and are 
able to serve them for reading and writing instead of writing to the terminal.

- You have to establish the inter process communication between different processes as 
shown in the figure above. 

## States Implementation
- The New process will read from a text file processes.txt. The processes file contains information about which scheduling algorithm to run and incoming processes (known as Procs) – the sample file is provided below. The intial row(s) contains the choice of  scheduling algorithm whereas the next rows contain information about Procs (i.e.  Arrival time, Burst time and Priority). The priority in case of non-priority algorithm  will be 0 for all Procs. 

- The New process will create a data structure for each Proc holding process information. Each Proc will also hold its execution information  (waiting time, completion time etc.)

- The Ready process is the core process of the simulator. The incoming Procs from New / Blocked / Running processes will enter the ready queue maintained by the Ready process for scheduling. The queue can be in form of a simple queue, multiple queues and min heap (depending on choice of scheduling). The scheduler will dispatch the processes to the Running processes as per the scheduling policy selected. The Ready process will wait from input from New, Blocked and Running processes, as well  notification from Running process to send next Proc for execution.

- The Running process mimics a uniprocessor CPU. Initially the time in CPU is 0. For simplicity of time management assume 1 tick of time to be simulated by sleep (1). Scheduler increments time until the time = arrival time of the first Proc. There will be a random binary number generated after 5 ticks. If binary number is 1 the current Proc continues its execution, in case of binary 0 the Proc is sent to blocked queue for I/O operation.In case of preemptive scheduling, Proc will be sent back to Ready queue upon completion of its time slice. In case, the Proc completes its execution, it will be sent to the Exit process, and Ready process will be notified to dispatch next Proc for  execution.

- he Blocked process will maintain a blocked queue. Like the Running process, assume 1 tick of time to be simulated by sleep (1). Each Proc in the blocked queue will be blocked for 15 – 25 ticks (randomly decided). Upon completion of the ticks, the Proc will be sent back to the Ready queue.

- The Exit process will accept all the completed Procs. It will output different Procstatistics to a text file named processes_stats.txt and also display about completion of that process on output screen. For each completed Proc, it will print Gantt chart with information stating Arrival time, Burst time, Turnaround time and Waiting time. After every 30 ticks, the Exit process will append the cumulative stats to file containing Throughput, Average Turn around and Average Waiting time.

### Algorithms
- CFS algorithm using queues. 
- RR algorithm using queues.
- SJF algorithm using heaps.
- SRTF algorithm using heaps

## Multithreading Implementation
- At the moment the Ready process has to wait for three different events i.e. incoming Procs from New, Event completed Procs from Blocked and Timed-out Procs from Running. This will decrease the efficiency of the scheduler, as all three queues will be needed to be checked at regular intervals for incoming Procs.. Multithreading is implemented in Ready process such that waiting activities are separated from core tasks. 

## Execution Instructions
- Compile and create executable of new.cpp, ready.cpp, running.cpp, blocked.cpp and exit.cpp
- Compile, create executable of 5_state_model_simulator.cpp and run the executable
