#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LENGTH 100

int main()
{
char input[MAX_LENGTH];

while (1)
{
// Display the prompt
printf("prompt> ");

// Read user input
if (fgets(input, sizeof(input), stdin) == NULL)
{
perror("ERROR READIN INPUT");
exit(EXIT_FAILURE);
}

// Remove newline character if present
input[strcspn(input, "\n")] = '\0';

// Check if the user wants to exit
if (strcmp(input, "exit") == 0)
{
break;
}

// Fork a child process
pid_t child = fork();

if (child == -1)
{
perror("ERROR CREATING CHILD");
exit(EXIT_FAILURE);
}
else if (child == 0)
{ // Child process

// Parse the command and arguments
char *args[MAX_LENGTH];
int count = 0;

char *token = strtok(input, " ");
while (token != NULL)
{
args[count++] = token;
token = strtok(NULL, " ");
}
args[count] = NULL;

// Execute the command in the child process
if (execvp(args[0], args) == -1)
{
perror("ERROR IN EXECUTING COMMAND IN CHILD");
exit(EXIT_FAILURE);
}
}
else
{ // Parent process
// Wait for the child to terminate
int status;
waitpid(child, &status, 0);

if (WIFEXITED(status))
{
printf("Child process terminated with status %d\n", WEXITSTATUS(status));
}
else
{
printf("Child process terminated abnormally\n");
}
}
}

return 0;
}
