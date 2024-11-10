#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int pid = fork();

    if (pid == 0) {  // Child process
        printf(1, "Child process started with PID %d\n", getpid());
        // Run an infinite loop to keep the process alive for testing
        while (1) {
            // Busy loop to keep process running
        }
    } else {  // Parent process
        sleep(10);  // Give the child process time to start
        printf(1, "Changing nice value of child process (PID %d) to 2\n", pid);
        // Set nice value of child to 2
        nice(pid, 2);
        sleep(10);  // Allow time to observe any scheduling effects

        printf(1, "Changing nice value of child process (PID %d) to 3\n", pid);
        // Change nice value to 3
        nice(pid, 3);
        sleep(10);

        printf(1, "Test complete. Killing child process.\n");
        kill(pid);
        wait();
    }

    exit();
}
