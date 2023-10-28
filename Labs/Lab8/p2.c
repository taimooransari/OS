#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    char message[100]; // Adjust message size as needed

    // Open the named pipe for reading
    fd = open("/tmp/chat_fifo", O_RDONLY);

    while (1) {
        // Read a message from the named pipe
        read(fd, message, sizeof(message));

        // Print the received message
        printf("P2 (Received): %s", message);
    }

    // Close the named pipe (unreachable in this example)
    close(fd);

    return 0;
}
