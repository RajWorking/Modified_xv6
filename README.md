# Modifying the xv6 Operating System

Note: This is an extension of the original xv6 operating system forked from 
**https://github.com/mit-pdos/xv6-public**

### Waitx System Call
`int waitx(int* wtime, int* rtime)`  
'rtime' is increased in trap.c. On each tick, if the process currently holding the cpu is in running state, running time is increased. Also, in each tick,  we loop over all processes in the ptable to increase the waiting time for all processes who are 'RUNNABLE' using function: `void inc_waiting()`  
Implementation of waitx system call is very similar to wait. 
It is used in user program `timechk` which executes any shell command and reports the running and waiting time for it.

### ps
`void proc_info()`  
This user program, reports all basic information about the active process on the system. Depending on the scheduler, it also specifies the priority of each process and in case of MLFQ, it shows the current queue of the process.
'q_ticks' is valid on in case of MLFQ, for others, it is -1.

### set_priority
`int set_priority(int new_priority, int pid)`  
This system call is used to change the priority of a process ranging 0-100. It is relevant only when scheduler is PBS and is not allowed with MLFQ scheduler.

### exec_proc
`void exec_proc(struct proc *p, struct cpu *c)`  
This function handles the execution of a process after one has been selected by the scheduler. Here we __n_run++__ to increase number of times the process was selected by scheduler. 

## Schedulers

### FCFS (First Come First Serve)
Simply loop over all process in the ptable and find the one with the minimum arrival time.

|Process |R-time  |W-time  |
|--------|--------|--------|
|P1      |1635    |0       |
|P2      |1324    |1635    |
|P3      |864     |2959    |
|P4      |419     |3823    |

* Shorter run-time than other schedulers but more waiting time so less responsiveness

### RR (Round Robin)
This is the default scheduler which was already implemented in the xv6 original.

|Process |R-time  |W-time  |
|--------|--------|--------|
|P1      |1816    |6       |
|P2      |1365    |3       |
|P3      |912     |1       |
|P4      |471     |0       |

* nearly equal distribution among waiting times

### PBS (Priority Base Scheduling)
Here we assign priorities (default 60) to each process and select the minimum one to execute. Since, many process may have same priority, we age the process after selecting it. Once all processes have been aged, we reset all back to 0.

|Process |R-time  |W-time  | Priority  |
|--------|--------|--------|-----------|
|P1      |2494    |0       |  70       |
|P2      |1598    |2193    |  71       |
|P3      |879     |4941    |  72       |
|P4      |424     |0       |  70       |

* waiting time is higher for processes with higher priority.

### MLFQ (Multi-Level Feedback Queue)
Here we create 5 queues with different levels of priority. In each queue, the process gets a timeslice of 2<sup>priority</sup>. Queues have been implemented using structs. Push/Pop operations were also defined.
```
struct node {  
  struct node *next;  
  struct proc *proc;  
};  
struct Queue  
{  
  struct node *head;  
  struct node *tail;  
  int len;
};
```
`Aging` was done by poping the process in the queues and pushing to lower priority queue.  
The process was sent to `yield` in trap.c when it exhausted its timeslice and was demoted to higher priority queue.  
In each iteration, the processes which were 'RUNNABLE' and not in some queue, were pushed into the queues as per their priority. 

|Process |R-time  |W-time  |
|--------|--------|--------|
|P1      |5560    |18      |
|P2      |3095    |11      |
|P3      |1500    |11      |
|P4      |497     |8       |

#### Exploitation
When the process gets alloted a queue, its cpu time gets reset to 0. The demotion of the process only happens when its cpu time exceeds the time slice of that queue. But what if the process SLEEPS (or goes for io) before its time is up? It simply leaves the cpu and later gets pushed back into the same queue. In this way, it avoids getting demoted from its queue for time longer than it is supposed to.

## Observations:

![Graph for 1 cpu](graph.png?raw=true "Title")
![Graph for 4 cpu](graph2.png?raw=true "Title")
