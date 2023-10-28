#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

int main() {
    int numIntegers = 1024;
    size_t size = numIntegers * sizeof(int);

    // Specify the virtual address where you want to map the memory.
    // You can choose any address that's not already in use.
    void* desiredAddress = (void*)0x54321000; // Replace with your desired address.

    // Allocate and map memory.
    int* virtualAddress = (int*)mmap(desiredAddress, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

    if (virtualAddress == MAP_FAILED) {
        perror("Memory mapping failed");
        return 1;
    }

    // Write values to the allocated memory.
    for (int i = 0; i < numIntegers; i++) {
        virtualAddress[i] = i * 2;
    }

    // Verify the values by reading them back.
    for (int i = 0; i < numIntegers; i++) {
        if (virtualAddress[i] != i * 2) {
            printf("Verification failed at index %d\n", i);
            break;
        }
    }

    // Unmap the memory.
    if (munmap(virtualAddress, size) == -1) {
        perror("Memory unmapping failed");
        return 1;
    }

    printf("Memory allocation and verification successful.\n");
    return 0;
}
