#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 4

int main()
{
    int pid_child;
    int child_number;

    printf("Parent process started, (PID: %d)\n", getpid());

    for (child_number = 0; child_number < NUM_CHILDREN; child_number++)
    {
        pid_child = fork();

        if (pid_child < 0)
        {
            perror("fork failed");
            return 1;
        }

        if (pid_child == 0)
        {
            printf("Child process %d created, (PID: %d), (PPID: %d)\n", child_number + 1, getpid(), getppid());


            sleep(1);

            return 0;
        }

        if (pid_child > 0)
        {
            int status;
            waitpid(pid_child, &status, 0);
            printf("Child process %d exited\n", child_number + 1);
        }
    }

    return 0;
}
