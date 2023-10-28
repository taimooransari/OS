#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd_read, fd_write;
    char message_read[100];
    char message_write[100];

    // Create a named pipe for reading if it doesn't exist
    mkfifo("/tmp/chat_fifo_p2", 0666);
    // Create a named pipe for writing if it doesn't exist
    mkfifo("/tmp/chat_fifo_p1", 0666);

    // Open the named pipe for writing
    fd_write = open("/tmp/chat_fifo_p1", O_WRONLY);

    // Open the named pipe for reading
    fd_read = open("/tmp/chat_fifo_p2", O_RDONLY);

    while (1) {
        // Read a message from the user
        printf("P2 (You): ");
        fgets(message_write, sizeof(message_write), stdin);

        // Write the message to the other process
        write(fd_write, message_write, sizeof(message_write));

        // Read a message from the other process
        read(fd_read, message_read, sizeof(message_read));

        // Print the received message
        printf("P2 (Received): %s", message_read);
    }

    // Close the named pipes (unreachable in this example)
    close(fd_read);
    close(fd_write);

    return 0;
}
