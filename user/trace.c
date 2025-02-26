// tracing system calls

#include "kernel/types.h"
#include "user/user.h"

int log_2(int x)
{
    int i = 0;
    while (x >>= 1)
    {
        i++;
    }
    return i;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(2, "Usage: trace <syscall_number> <command> <args>\n");
    exit(1);
  }

    int trace_mask = atoi(argv[1]);

    if (trace(trace_mask) < 0)
    {
        fprintf(2, "trace failed\n");
        exit(1);
    }

    if (exec(argv[2], argv + 2) < 0)
    {
        fprintf(2, "exec failed\n");
        exit(1);
    }

    return 0;
}
