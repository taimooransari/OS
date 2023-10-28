#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h> // Include for wait function
#include <string.h>
int main(void) {
    int pipefd[2];
    int pid;
    char buffer[600];

    pipe(pipefd);
    pid = fork();

    if (pid > 0) {
        // Parent process
        fflush(stdin); // Flush standard input
        close(pipefd[1]); // Close the write end of the pipe
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Unnamed pipe [INFO] Parent Process: %s\n", buffer);
    } else if (pid == 0) {
        // Child process
        sleep(5); // Sleep to simulate some work
        fflush(stdin); // Flush standard input
        close(pipefd[0]); // Close the read end of the pipe
	printf("Unnamed pipe [INFO] Child Process: %s\n", buffer);

        const char* message = "Hello, this is a message from the Child!";
        write(pipefd[1], message, strlen(message) + 1);
    } else {
        printf("Unnamed pipe [ERROR] Error creating child process\n");
    }

    if (pid > 0) {
        wait(NULL);
    }

    return 0;
}

