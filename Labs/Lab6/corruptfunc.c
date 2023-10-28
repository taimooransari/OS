#include <stdio.h>

void overwriteReturnAddress() {
    printf("Inside overwriteReturnAddress()\n");
    
    // Create a pointer to the stack frame
    unsigned long *framePtr = (unsigned long *)__builtin_frame_address(0);
    
    // Overwrite the return address with a known address (e.g., 0x41414141)
    framePtr[1] = 0x41414141;
    
    // The function will not return to the original caller
}

int main() {
    printf("Before calling overwriteReturnAddress()\n");
    
    overwriteReturnAddress(); // Call the function to overwrite the return address
    
    printf("After calling overwriteReturnAddress()\n");
    
    return 0;
}
