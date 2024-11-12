#include "types.h"
#include "stat.h"
#include "user.h"

// Function to set scheduler mode (0 = Round Robin, 1 = Priority-based)
void set_scheduler_mode(int mode) {
    setschedmode(mode); // Assuming this system call is implemented to set the scheduler mode
}

// Delay function based on nice value
void staggered_delay(int nice_value) {
    for (volatile int i = 0; i < nice_value * 5000000; i++) {
        asm volatile("nop"); // No operation to induce delay
    }
}

// CPU-intensive task to demonstrate priority scheduling
void cpu_task(int nice_value) {
    int i, j, count = 0;
    printf(1, "\nProcess with nice=%d started.\n", nice_value);

    for (i = 2; i < 10000; i++) {
        int is_prime = 1;
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            count++;
            if (count % 500 == 0) {  // Progress update every 500 primes
                printf(1, "Process with nice=%d has found %d primes so far.\n", nice_value, count);
            }
        }
    }

    printf(1, "Process with nice=%d completed. Total primes found: %d\n", nice_value, count);
}

int main() {
    int pid;
    int num_processes = 5;
    int nice_values[] = {1, 2, 3, 4, 5};

    // Set scheduler mode to priority-based
    set_scheduler_mode(1); // Set to priority-based scheduling

    // Fork child processes and assign different nice values
    for (int i = 0; i < num_processes; i++) {
        pid = fork();
        if (pid < 0) {
            printf(1, "Fork failed\n");
            exit();
        } else if (pid == 0) {
            // In child process: set nice value, delay, and perform CPU-intensive task
            int current_pid = getpid();
            nice(current_pid, nice_values[i]);
            staggered_delay(nice_values[i]); // Stagger start based on priority
            cpu_task(nice_values[i]);
            exit();
        }
    }

    // Parent process waits for all child processes to complete
    for (int i = 0; i < num_processes; i++) {
        wait();
    }

    printf(1, "\nAll child processes have completed.\n");
    exit();
}
