#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pid, new_nice_value;
    int result;

    if (argc == 2) {
        // Only nice value provided, set for current process
        pid = 0; // Set pid to 0 to indicate current process
        new_nice_value = atoi(argv[1]);
    } else if (argc == 3) {
        // Both pid and nice value provided
        pid = atoi(argv[1]);
        new_nice_value = atoi(argv[2]);
    } else {
        printf(2, "Usage: nice <pid> <value> or nice <value>\n");
        exit();
    }

    // Call the nice system call
    result = nice(pid, new_nice_value);
    
    if (result < 0) {
        printf(2, "Failed to set nice value\n");
    } else {
        int old_pid = (result >> 16) & 0xFFFF; // Extract pid
        int old_nice_value = result & 0xFFFF;   // Extract old nice value
        printf(1, "%d %d\n", old_pid, old_nice_value);
    }

    exit();
}
