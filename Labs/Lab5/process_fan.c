#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("Hello world (pid: %d)\n", (int)getpid());

    for (int x = 0; x < 4; x++)
    {
        int rc = fork();

        if (rc < 0)
        {
            // Fork failed
            fprintf(stderr, "Fork failed\n");
            exit(1);
        }
        else if (rc == 0)
        {
            // Child(new process)
            printf("I am child %d (pid: %d)\n", x + 1, (int)getpid());
            exit(0); // Child process exit
        }
        else
        {
            // Parent waits for the child to terminate
            int wc = wait(NULL);
            printf("I am parent (pid: %d), and I waited for child %d (pid: %d, wc: %d)\n", (int)getpid(), x + 1, rc, wc);
        }
    }

    return 0;
}
