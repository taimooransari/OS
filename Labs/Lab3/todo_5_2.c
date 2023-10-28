#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    FILE *stream = fopen("helloworld.txt", "a");

    if (stream == NULL)
    {
        printf("Error opening file");
        return 1;
    }
    fprintf(stream, "Hello world!\n");

    fclose(stream);
    return 0;
}
