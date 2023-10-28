#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {

        printf("Provide filename for source and destination properly.");
        return 1;
    }

    const char *source_filename = argv[1];
    const char *destination_filename = argv[2];

    FILE *source_file = fopen(source_filename, "rb");

    if (source_file == NULL)
    {
        printf("Error opening source file.");
        return 1;
    }

    FILE *destination_file = fopen(destination_filename, "wb");

    if (destination_file == NULL)
    {
        printf("Error opening destination file.");
        fclose(source_file);
        return 1;
    }

    char buffer[1024];
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source_file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, destination_file);
    }

    fclose(source_file);
    fclose(destination_file);

    printf("File copied successfully.");

    return 0;
}
