// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  int time_start;              // Start time of process
  int time_run;                // Run time of process
  int time_end;                // End time of process
  int time_wait;               // Waiting time for cpu in queue
  int total_wait;              // Net total waiting time of process

  int n_run;                   // number of times alloted cpu
  int priority;                // priority of process
  int age;                     // age of process (PBS)

  int in_queue;                // is the process in some queue? 
  int q_ticks[5];              // ticks spent in the 5 queues

  // current queue of a process is gotten by 'priority'
  // timeslice of that queue is (1 << priority)
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap

// Scheduling algorithms used by scheduler
#define RR 0
#define FCFS 1
#define PBS 2
#define MLFQ 3

// nodes of any queue of processes
struct node
{
  struct node *next;
  struct proc *proc;
};
// queues for the different levels of mlfq scheduling
struct Queue
{
  struct node *head;
  struct node *tail;
  int len;
};

struct node free_nodes[NPROC];
struct Queue queues[5]; // queues for mlfq scheduling

#define WAIT_LIMIT 10