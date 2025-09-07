# CPU_Scheduler

## Overview: 🗣️ 

This project simulates the behavior of a CPU scheduler, the part of an operating system responsible for 
deciding which process runs next on the CPU. The simulator takes a list of processes—each with attributes
like arrival time, burst time, and priority—and runs them through multiple scheduling algorithms, including:
- First-Come, First-Served (FCFS)
- Shortest Job First (SJF)
- Priority Scheduling
- Round Robin
  
For each algorithm, the simulator calculates and displays important performance metrics such as:
- Average Waiting Time – how long processes wait in the queue before execution
- Average Turnaround Time – the total time from arrival to completion

This project gives a high-level view of how different scheduling strategies affect the efficiency and fairness of CPU time allocation. While simplified, it captures the core concepts used in real operating systems to manage multitasking and process execution.
