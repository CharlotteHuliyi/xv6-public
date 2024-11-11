// #include "types.h"
// #include "stat.h"
// #include "user.h"

// int compute_primes(int limit) {
//     int i = 2;
//     int prime_count = 0;

//     while (prime_count < limit) {
//         int is_prime = 1;
//         for (int j = 2; j * j <= i; j++) {
//             if (i % j == 0) {
//                 is_prime = 0;
//                 break;
//             }
//         }
//         if (is_prime) {
//             prime_count++;
//         }
//         i++;
//     }
//     return prime_count;
// }

// int main(void) {
//     int nice_values[] = {1, 3, 5};       // Nice values representing priority
//     int limits[] = {2000, 1500, 1000};   // Workload limits for each child

//     // Fork child processes
//     for (int i = 0; i < 3; i++) {
//         if (fork() == 0) {   // Child process
//             nice(0, nice_values[i]);     // Set nice value based on priority
//             int prime_count = compute_primes(limits[i]); // Compute primes up to limit
//             printf(1, "Process with nice %d finished with %d primes calculated.\n", nice_values[i], prime_count);
//             sleep(10); // Small delay to reduce output overlap
//             exit(); // Exit after computation
//         }
//     }

//     // Parent waits for each child process to complete
//     for (int i = 0; i < 3; i++) {
//         wait(); // Parent waits for each child to complete
//     }

//     printf(1, "Test4 complete. Results should reflect priority differences in CPU allocation.\n");
//     exit();
// }


#include "types.h"
#include "stat.h"
#include "user.h"

int compute_primes(int limit) {
    int i = 2;
    int prime_count = 0;

    while (prime_count < limit) {
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

void run_test_with_scheduler_mode(int mode) {
    setschedmode(mode);  // Set the scheduler mode: 0 for Round Robin, 1 for Priority-based
    printf(1, "Running test4 with scheduler mode %s.\n", mode == 1 ? "Priority-based" : "Round Robin");

    int nice_values[] = {1, 3, 5};       // Nice values representing priority
    int limits[] = {2000, 1500, 1000};   // Workload limits for each child

    // Fork child processes
    for (int i = 0; i < 3; i++) {
        if (fork() == 0) {   // Child process
            nice(0, nice_values[i]);     // Set nice value based on priority
            int prime_count = compute_primes(limits[i]); // Compute primes up to limit
            printf(1, "Process with nice %d finished with %d primes calculated.\n", nice_values[i], prime_count);
            sleep(10); // Small delay to reduce output overlap
            exit(); // Exit after computation
        }
    }

    // Parent waits for each child process to complete
    for (int i = 0; i < 3; i++) {
        wait(); // Parent waits for each child to complete
    }

    printf(1, "Test4 complete in %s mode.\n", mode == 1 ? "Priority-based" : "Round Robin");
}

int main(void) {
    // Run test with Round Robin mode
    run_test_with_scheduler_mode(0);

    // Run test with Priority-based Round Robin mode
    run_test_with_scheduler_mode(1);

    printf(1, "Test4 finished. Compare results between Round Robin and Priority-based modes.\n");
    exit();
}
