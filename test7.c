#include "types.h"
#include "stat.h"
#include "user.h"
// Set scheduler mode function (0 for Round Robin, 1 for Priority-based)
void set_scheduler_mode(int mode) {
    setschedmode(mode); 
}
// Function to compute primes within a given time limit
int compute_primes(int time_limit) {
    int i = 2;
    int prime_count = 0;
    int start_time = uptime();

    while ((uptime() - start_time) < time_limit) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_count++;
        }
        i++;
    }
    return prime_count;
}

int main(void) {
    set_scheduler_mode(1);        // Set scheduler to priority-based mode

    int initial_nice = 3;         // Start with the same nice value for all processes
    int time_interval = 500;      // Each interval for prime computation
    int updated_nice[] = {1, 3, 5};  // New nice values after first interval

    // Fork child processes with initial equal priority
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {         // Child process
            nice(0, initial_nice); // Set initial nice value
            int primes_first_half = compute_primes(time_interval);  // First interval

            // Adjust priority dynamically
            nice(0, updated_nice[i]);
            yield();               // Yield to enforce re-scheduling with new priority

            int primes_second_half = compute_primes(time_interval); // Second interval
            printf(1, "Process %d: First half=%d primes, Second half=%d primes, Final nice=%d\n",
                    getpid(), primes_first_half, primes_second_half, updated_nice[i]);
            exit(); // Exit after computation
        }
    }

    // Parent waits for each child process to complete
    for (int i = 0; i < 3; i++) {
        wait();
    }

    printf(1, "Test7 complete. Results should reflect impact of dynamic priority adjustment.\n");
    exit();
}
