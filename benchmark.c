#include "types.h"
#include "user.h"

int number_of_processes = 10;

int main(int argc, char *argv[])
{
  set_priority(0, getpid());
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
          sleep(20); //io time
        }
        else
        {
          for (i = 0; i < 1000000000; i++)
          {
            ; //cpu time
          }
        }
      }
      printf(1, "Process Finished with pid: ---> %d\n", j, getpid());
      exit();
    }
    else
    {
      set_priority(70 + j % 3, pid);
    }
  }
  // for (j = 0; j < number_of_processes + 5; j++)
  // {
  //   wait();
  // }
  exit();
}
