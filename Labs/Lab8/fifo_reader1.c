#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd;
    char buffer[100]; // Adjust buffer size as needed

    fd = open("/tmp/myfifo", O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    printf("Reader 1 received: %s\n", buffer);
    close(fd);

    return 0;
}

