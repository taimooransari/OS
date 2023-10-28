#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    char message[100]; // Adjust message size as needed

    // Create a named pipe for communication
    mkfifo("/tmp/chat_fifo", 0666);

    // Open the named pipe for writing
    fd = open("/tmp/chat_fifo", O_WRONLY);

    while (1) {
        // Read a message from the user
        printf("P1 (You): ");
        fgets(message, sizeof(message), stdin);

        // Write the message to the named pipe
        write(fd, message, sizeof(message));
    }

    // Close the named pipe (unreachable in this example)
    close(fd);

    return 0;
}
