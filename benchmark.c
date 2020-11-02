#include "types.h"
#include "user.h"

int number_of_processes = 7;

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
              sleep(50); //io time
        } 
        else
        {
          for (i = 0; i < 1e7; i++)
          {
            ; //cpu time
          }
        }
      }
      printf(1, "Process Finished with pid: ---> %d\n", getpid());
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
