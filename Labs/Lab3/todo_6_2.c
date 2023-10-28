#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *stream = fopen("processes.dat", "r");
    if (stream == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    char line[1024];
    char *token;

    while (fgets(line, sizeof(line), stream) != NULL)
    {
        int val = 0;
        char priority[32];
        char id[32];

        token = strtok(line, ":");
        while (token != NULL)
        {
            if (val == 0)
            {
                strcpy(id, token);
            }
            else if (val == 3)
            {
                strcpy(priority, token);
            }
            token = strtok(NULL, ":");
            val++;
        }

        printf("Process ID: %s, Priority: %s\n", id, priority);
    }

    fclose(stream);
    return 0;
}
