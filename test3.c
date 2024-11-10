#include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
    int initial_nice = 1;
    int new_nice = 2;

    // Set nice value on the current process
    printf(1, "Setting nice value to %d on current process\n", initial_nice);
    nice(0, initial_nice);

    printf(1, "Current process nice value set to %d\n", initial_nice);
    sleep(10);

    printf(1, "Changing nice value to %d on current process\n", new_nice);
    nice(0, new_nice);
    printf(1, "Current process nice value changed to %d\n", new_nice);
    sleep(10);

    printf(1, "Test complete.\n");
    exit();
}
