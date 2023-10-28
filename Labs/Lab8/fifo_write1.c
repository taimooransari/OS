#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    char buffer[] = "Hello from fifo_write!"; // Message to send

    fd = open("/tmp/myfifo", O_WRONLY);
    write(fd, buffer, sizeof(buffer));
    close(fd);

    return 0;
}

