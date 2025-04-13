#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

char* format_ind(int ind, char* buf) {
  buf[3] = 0;
  if (ind % 16 > 9)
    buf[2] = 'a' + (ind % 16) - 10;
  else
    buf[2] = '0' + (ind % 16);
  ind /= 16;
  if (ind % 16 > 9)
    buf[1] = 'a' + (ind % 16) - 10;
  else
    buf[1] = '0' + (ind % 16);
  ind /= 16;
  if (ind % 16 > 9)
    buf[0] = 'a' + (ind % 16) - 10;
  else
    buf[0] = '0' + (ind % 16);
  return buf;
}

void print_pgtable(pagetable_t pgtable, uint64 i, int level, int flags) {

  char buf[4];
  pte_t pte = pgtable[i];
  
  if (level > 0) {

    for (int j = 0; j < (2 - level) * 9; j++)
      printf(".");

    if (level == 1)
      printf(" ");

    printf("0x%s -> %p\n", format_ind(i, buf), (void*)PTE2PA(pte));
    return;
  }

  if (flags && ((pte & flags) == 0))
    return;

  for (int j = 0; j < (2 - level) * 9; j++)
    printf(".");
  printf(".");

  printf("0x%s -> %p ", format_ind(i, buf), (void*)PTE2PA(pte));

  printf("%s", (pte & PTE_R) ? "R" : "_");
  printf("%s", (pte & PTE_W) ? "W" : "_");
  printf("%s", (pte & PTE_X) ? "X" : "_");
  printf("%s", (pte & PTE_U) ? "U" : "_");
  printf("%s", (pte & PTE_G) ? "G" : "_");
  printf("%s", (pte & PTE_A) ? "A" : "_");
  printf("%s\n", (pte & PTE_D) ? "D" : "_");
}

void visit_pgtable(pagetable_t pgtable, uint64 l, uint64 r, int level, int flags, int type, uint64 cur_va) {
  for (int i = 0; i < 512; i++) {
    uint64 va = cur_va | (uint64)i << PXSHIFT(level);

    if ((va + (1UL << PXSHIFT(level)) <= l) || (r <= va))
      continue;

    if (!(pgtable[i] & PTE_V))
      continue;

    if (type == 1)
      print_pgtable(pgtable, i, level, flags);

    if (level > 0)
      visit_pgtable((pagetable_t)PTE2PA(pgtable[i]), l, r, level - 1, flags, type, va);
    else if (type == 2) {
      pgtable[i] &= ~flags;
    }
  }
}

uint64 sys_print_pgtable(void) {
  uint64 buf;
  argaddr(0, &buf);
  uint64 len;
  argaddr(1, &len);

  if (buf + len > MAXVA) {
    return -1;
  }

  if (buf == 0 || len == 0) {
    buf = 0;
    len = MAXVA;
  }
  
  int flags;
  argint(2, &flags);

  if (flags == 1)
    flags = PTE_D;
  else if (flags == 2)
    flags = PTE_A;
  else if (flags == 3)
    flags = PTE_D | PTE_A;
  else if (flags != 0)
    return -1;

  pagetable_t pagetable = myproc()->pagetable;
  printf("PAGETABLE %p\n", pagetable);
  visit_pgtable(pagetable, buf, buf + len, 2, flags, 1, 0);
  return 0;
}

uint64 sys_reset_flags(void) {
  uint64 buf;
  argaddr(0, &buf);
  uint64 len;
  argaddr(1, &len);

  if (buf + len > MAXVA) {
    return -1;
  }

  if (buf == 0 || len == 0) {
    buf = 0;
    len = MAXVA;
  }

  int flags;
  argint(2, &flags);

  if (flags == 1)
    flags = PTE_D;
  else if (flags == 2)
    flags = PTE_A;
  else if (flags == 3)
    flags = PTE_D | PTE_A;
  else if (flags != 0)
    return -1;

  pagetable_t pagetable = myproc()->pagetable;
  visit_pgtable(pagetable, buf, buf + len, 2, flags, 2, 0);
  return 0;
}
