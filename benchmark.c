#include "types.h"
#include "user.h"

// Scheduling algorithms used by scheduler
#define RR 0
#define FCFS 1
#define PBS 2
#define MLFQ 3

int number_of_processes = 5;

int main(int argc, char *argv[])
{
  #if SCHEDULER != MLFQ
  set_priority(0, getpid());
  #endif
  int j;
  for (j = 0; j < number_of_processes; j++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed\n");
      continue;
    }
    if (pid == 0)
    {
      volatile int i;
      for (volatile int k = 0; k < number_of_processes; k++)
      {
        if (k <= j)
        {
              sleep(100); //io time
        } 
        else
        {
          for (i = 0; i < 1e9; i++)
          {
            ; //cpu time
          }
        }
      }
      int yy = getpid();
      proc_info();
      printf(1, "Process Finished with pid: ---> %d\n", yy);
      exit();
    }
    else
    {
      #if SCHEDULER != MLFQ
        set_priority(70 + j % 3, pid);
      #endif
    }
  }
  // for (j = 0; j < number_of_processes + 5; j++)
  // {
  //   wait();
  // }
  exit();
}
