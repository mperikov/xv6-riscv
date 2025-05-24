#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"


#define Rtc(reg) ((volatile uint32 *)(reg))

#define ReadRtc(reg) (*(Rtc(reg)))

struct spinlock lock;

void rtcinit(void) {
  initlock(&lock, "rtc");
}

uint64 rtc() {

  acquire(&lock);
  uint64 low = ReadRtc(RTC_LOW), high = ReadRtc(RTC_HIGH);
  release(&lock);

  return low | (high << 32);
}

uint64 sys_rtc() {

  uint64 arg;
  argaddr(0, &arg);

  uint64 t = rtc();

  return copyout(myproc()->pagetable, arg, (char*)(&t), sizeof(t));
}
