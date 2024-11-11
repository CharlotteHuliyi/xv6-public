#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#include "spinlock.h"
extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

extern int scheduler_mode;


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_nice(void) {
  int pid, new_nice_value;

  // Fetch the pid and nice value arguments
  if (argint(0, &pid) < 0 || argint(1, &new_nice_value) < 0) {
    return -1; // Invalid arguments
  }

  // Bounds check for new nice value (should be between 1 and 5)
  if (new_nice_value < 1 || new_nice_value > 5) {
    cprintf("sys_nice: new_nice_value %d is out of bounds (must be between 1 and 5)\n", new_nice_value);
    return -1; // Invalid nice value
  }

  struct proc *p = 0;
  int old_nice_value = -1;

  acquire(&ptable.lock); // Lock the process table

  // If pid is 0, use the current process
  if (pid == 0) {
    p = myproc(); // Get the current process
  } else {
    // Otherwise, search for the process with the given pid
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
      if (p->pid == pid) {
        break;
      }
    }
  }

  if (p && p->state != UNUSED) { // Check if process exists and is valid
    old_nice_value = p->nice;  // Store old nice value
    p->nice = new_nice_value;  // Update nice value
  } else {
    release(&ptable.lock);
    return -1; // Process with given pid not found or invalid
  }

  release(&ptable.lock); // Unlock the process table

  // Return the pid and old nice value packed as a single integer
  return (p->pid << 16) | (old_nice_value & 0xFFFF);
}


int sys_setschedmode(void) {
    int mode;
    if (argint(0, &mode) < 0)
        return -1;
    if (mode != 0 && mode != 1)
        return -1;  // Only accept 0 or 1 as valid inputs
    scheduler_mode = mode;
    return 0;
}
