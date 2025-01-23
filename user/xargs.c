#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define BUFSIZE 256

// xargs: extended arguments

int read_line(char *buf, int bufsize)
{
    int n = 0;
    char c = 0;

    while (n < bufsize - 1)
    {
        // check if has read to the end
        if (read(0, &c, 1) != 1)
            break;
        
        if (c == '\n')
            break;
        buf[n++] = c;
    }

    // add NULL terminator
    buf[n] = 0;

    // return line's length
    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: xargs <command> [arguments...]\n");
        exit(1);
    }

    char line[BUFSIZE] = {0};
    char *xargs[MAXARG];
    int i = 0;
    for (; i < argc - 1; ++i)
    {
        // ignore "xargs", copy the command and arguments
        xargs[i] = argv[i + 1];
    }

    while (read_line(line, BUFSIZE))
    {
        xargs[i] = line;
        xargs[i + 1] = 0;

        if (fork() == 0)
        {
            exec(xargs[0], xargs);
            
            // in case something went wrong
            printf("exec failed\n");
            exit(1);
        }
        wait(0);
    }
    exit(0);
}