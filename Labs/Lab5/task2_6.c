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
// Check for output redirection
char *file_opt = NULL;
char *token_opt = strstr(input, ">");
if (token_opt != NULL)
{
*token_opt = '\0';
file_opt = strtok(token_opt + 1, " ");
if (file_opt != NULL)
{
// Trim leading and trailing spaces from the output file name
while (file_opt[0] == ' ')
{
file_opt++;
}
size_t len = strlen(file_opt);
while (len > 0 && file_opt[len - 1] == ' ')
{
file_opt[len - 1] = '\0';
len--;
}
}
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
// Handle output redirection if needed
if (file_opt != NULL)
{
int file_d = open(file_opt, O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (file_d == -1)
{
perror("error in opening file");
exit(EXIT_FAILURE);
}
dup2(file_d, STDOUT_FILENO);
close(file_d);
}
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
