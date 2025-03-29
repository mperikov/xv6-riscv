#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void out_args(int argc, char* argv[], int mutex, int fl) {
  for (int i = 1; i < argc; i++) {
    for (int j = 0; argv[i][j]; j++) {
      if (fl)
        mutex_lock(mutex);
      char c[] = {argv[i][j], 0};
      printf("%d: arg %d, char '%s'\n", getpid(), i, c);
      if (fl)
        mutex_unlock(mutex);
    }
  }
}

int main(int argc, char* argv[]) {
  int mtx;
  if (mutex(&mtx) < 0)
    exit(1);

  printf("Without mutex:\n");
  if (fork() == 0) {
    out_args(argc, argv, 0, 0);
    exit(0);
  }
  else {
    out_args(argc, argv, 0, 0);
    wait(0);
  }

  printf("With mutex:\n");
  if (fork() == 0) {
    out_args(argc, argv, mtx, 1);
    exit(0);
  }
  else {
    out_args(argc, argv, mtx, 1);
    wait(0);
  }

  close(mtx);
  exit(0);
}
