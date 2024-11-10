#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int pid = fork();

    if (pid == 0) {  // Child process
        printf(1, "Child process started with PID %d\n", getpid());
        while (1) {
            // Keep the process alive for testing
        }
    } else {  // Parent process
        sleep(10);

        // Attempt to set an invalid negative nice value
        printf(1, "Trying to set nice value to -1 (out of bounds)\n");
        int result = nice(pid, -1);
        if (result < 0) {
            printf(1, "Correctly failed to set out-of-bounds nice value (-1)\n");
        } else {
            printf(1, "Error: out-of-bounds value (-1) was accepted\n");
        }

        // Attempt to set an excessively high nice value (e.g., 50)
        printf(1, "Trying to set nice value to 50 (out of bounds)\n");
        result = nice(pid, 50);
        if (result < 0) {
            printf(1, "Correctly failed to set out-of-bounds nice value (50)\n");
        } else {
            printf(1, "Error: out-of-bounds value (50) was accepted\n");
        }

        // Clean up the child process
        printf(1, "Test complete. Killing child process.\n");
        kill(pid);
        wait();
    }

    exit();
}
