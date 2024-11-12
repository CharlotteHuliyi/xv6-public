#include "types.h"
#include "stat.h"
#include "user.h"
// Set scheduler mode function (0 for Round Robin, 1 for Priority-based)
void set_scheduler_mode(int mode) {
    setschedmode(mode); 
}

// Function to simulate a CPU-bound task with a spinlock delay
void spinlock_task(int spin_count) {
    volatile int i;
    for (i = 0; i < spin_count; i++) {
        asm volatile("nop");  // No operation to consume CPU cycles
    }
}

int main(void) {
    set_scheduler_mode(1);           // Set scheduler to priority-based mode

    int nice_values[] = {1, 3, 5};   // Nice values within the 1-5 boundary
    int spin_count = 500000000;      // Large spin count for a CPU-intensive task
    int staggered_start_delay[] = {5, 10, 15}; // Staggered start to reduce overlap

    // Fork child processes with different fixed priorities
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {           // Child process
            sleep(staggered_start_delay[i]); // Staggered start for readability

            // Set the nice value for each process
            nice(0, nice_values[i]);

            // Record start time
            int start_time = uptime();

            // Run CPU-bound task
            spinlock_task(spin_count);

            // Record end time and calculate total time taken
            int end_time = uptime();
            int total_ticks = end_time - start_time;

            // Print results with final nice value and time taken
            printf(1, "Process %d completed. Total ticks used=%d, Fixed nice=%d\n", getpid(), total_ticks, nice_values[i]);
            exit(); // Exit after computation
        }
    }

    // Parent waits for each child process to complete
    for (int i = 0; i < 3; i++) {
        wait(); // Parent waits for each child to complete
    }

    printf(1, "Test6 complete. Results should reflect impact of fixed priority differences.\n");
    exit();
}