#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "spinlock.h"
#include "fs.h"
#include "sleeplock.h"
#include "file.h"
#include "param.h"
#include "proc.h"

struct file* mutexalloc(void) {
  struct file* f = filealloc();
  if (f == 0) {
    printf("proc %d: mutexalloc failed\n", myproc()->pid);
    return 0;
  }

  f->mutex = (struct sleeplock*)kalloc();
  if (f->mutex == 0) {
    fileclose(f);
    printf("proc %d: mutexalloc failed\n", myproc()->pid);
    return 0;
  }

  f->type = FD_MUTEX;
  f->readable = 0;
  f->writable = 0;
  initsleeplock(f->mutex, "");
  printf("proc %d: mutex allocated\n", myproc()->pid);
  return f;
}

int mutexclose(struct file* f) {
  if (f == 0 || f->type != FD_MUTEX) {
    printf("proc %d: mutexclose failed\n", myproc()->pid);
    return -1;
  }

  if (holdingsleep(f->mutex)) {
    panic("mutexclose");
  }
  kfree((char*)f->mutex);
  printf("proc %d: mutex closed\n", myproc()->pid);
  return 0;
}
