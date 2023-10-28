#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("No file name provided to read.");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Error opening file");
        return 1;
    }

    int line_count = 0;
    char line[1024];

    while (line_count < 10 && fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
        line_count++;
    }

    fclose(file);

    return 0;
}
