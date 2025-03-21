typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

enum procstate_u {
  PSLEEPING, 
  PRUNNABLE, 
  PRUNNING, 
  PZOMBIE
};

struct procinfo {
  int pid;
  char name[16];
  int state;
  int parent_pid;
  char parent_name[16];
};
