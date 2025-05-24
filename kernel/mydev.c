#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#include "mydev.h"

struct {
  uint seed, a, b;
  uint64 cnt;
  struct spinlock lock_urandom;
  struct spinlock lock_nullstat;
} mydev;


int mydev_write(int user_src, uint64 src, int n, short minor) {
  int ret = -1;

  switch (minor) {

  case NULL:

    ret = n;

    break;

  case URANDOM:
    
    if (n != sizeof(mydev.seed))
      break;

    acquire(&mydev.lock_urandom);

    if (either_copyin((char*)&mydev.seed, user_src, src, n) == -1)
      ret = 0;
    else
      ret = n;

    release(&mydev.lock_urandom);

    break;

  case NULLSTAT:

    acquire(&mydev.lock_nullstat);

    mydev.cnt += n;

    release(&mydev.lock_nullstat);

    ret = n;

    break;

  default:
    break;
  }

  return ret;
}


char empty[EMPTY_SIZE];

int mydev_read(int user_dst, uint64 dst, int n, short minor) {
  int ret = -1;

  switch (minor) {

    case NULL:

      ret = 0;

      break;

    case ZERO:

      ret = n;
      int dlt;
      while (n > 0) {

        dlt = n;
        if (dlt > EMPTY_SIZE)
          dlt = EMPTY_SIZE;

        n -= dlt;

        if (either_copyout(user_dst, dst, (char*)empty, dlt) == -1) {
          ret = -1;
          break;
        }
      }
      break;
    case URANDOM:

      if (n != sizeof(mydev.seed))
        break;

      acquire(&mydev.lock_urandom);
    
      mydev.seed *= A;
      mydev.seed += B;

      if (either_copyout(user_dst, dst, (char*)&mydev.seed, n) == -1)
        ret = 0;
      else
        ret = n;

      release(&mydev.lock_urandom);
      break;

    case NULLSTAT:

      if (n != sizeof(mydev.cnt))
        break;

      acquire(&mydev.lock_nullstat);

      if (either_copyout(user_dst, dst, (char*)&mydev.cnt, n) == -1)
        ret = 0;
      else
        ret = n;

      release(&mydev.lock_nullstat);
      break;

    default:
      break;
  }
  return ret;
}

void
mydevinit(void) {

  for (int i = 0; i < EMPTY_SIZE; ++i)
    empty[i] = 0;

  mydev.seed = 30;
  mydev.cnt = 0;

  initlock(&mydev.lock_urandom, "mydev_urandom");
  initlock(&mydev.lock_nullstat, "mydev_nullstat");

  devsw[MYDEV].read = mydev_read;
  devsw[MYDEV].write = mydev_write;
}
