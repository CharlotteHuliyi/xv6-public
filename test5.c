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
    int nice_value = 3;          // Set all processes to the same nice value for equal priority
    int time_limit = 100;         // Run each process for 100 ticks

    // Fork child processes
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {   // Child process
            nice(0, nice_value);      // Set the same nice value for each child
            sleep(5);
            int prime_count = compute_primes(time_limit); // Compute primes within time limit
            printf(1, "Process %d with nice %d calculated %d primes in %d ticks.\n", getpid(), nice_value, prime_count, time_limit);
            sleep(10); 
            exit(); // Exit after computation
        }
    }

    // Parent waits for each child process to complete
    for (int i = 0; i < 3; i++) {
        wait(); // Parent waits for each child to complete
    }

    printf(1, "Test5 complete. Results should show similar prime counts for equal CPU allocation.\n");
    exit();
}
