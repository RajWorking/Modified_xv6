#include "types.h"
#include "user.h"

int main(int argc, char **argsv)
{
    if (argc != 3)
    {
        printf(2, "Incorrect way of calling setPriority :(\n");
        exit();
    }

    set_priority(atoi(argsv[1]),atoi(argsv[2]));

    exit();
}