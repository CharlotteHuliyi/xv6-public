#include "types.h"
#include "stat.h"
#include "user.h"

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
    int initial_nice = 3;        // Start with the same nice value for all processes
    int time_limit = 100;        // Run each process for 100 ticks initially

    // Fork child processes
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {   // Child process
            nice(0, initial_nice);      // Set the initial nice value
            int primes_first_half = compute_primes(time_limit); // Compute primes for first half

            // Adjust nice value mid-computation
            if (i == 0) nice(0, 1);     // Set highest priority for process 0
            else if (i == 1) nice(0, 5); // Set lowest priority for process 1
            // Process 2 keeps the same nice value

            int primes_second_half = compute_primes(time_limit); // Compute primes for second half
            printf(1, "Process %d: First half=%d primes, Second half=%d primes, Final nice=%d\n", getpid(), primes_first_half, primes_second_half, i == 0 ? 1 : (i == 1 ? 5 : 3));
            sleep(10); // Small delay to reduce output overlap
            exit(); // Exit after computation
        }
    }

    // Parent waits for each child process to complete
    for (int i = 0; i < 3; i++) {
        wait(); // Parent waits for each child to complete
    }

    printf(1, "Test6 complete. Results should reflect dynamic priority changes.\n");
    exit();
}
