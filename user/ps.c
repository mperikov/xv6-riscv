#include "kernel/types.h"
#include "user/user.h"
#include <stdbool.h>

int main() {
  struct procinfo* buff;
  int buff_size = 1;
  int p_count;

  while (true) {
    buff = malloc(buff_size * sizeof(struct procinfo));
    if (buff == 0) {
      fprintf(2, "malloc failed\n");
      exit(1);
    }
    p_count = ps_listinfo(buff, buff_size);
    if (p_count < 0) {
      fprintf(2, "ps_listinfo failed\n");
      exit(1);
    }

    if (p_count <= buff_size)
      break;
    free(buff);
    buff_size <<= 1;
  }

  printf("pid\tname\tstate   \tppid\tpname\n");
  for (struct procinfo *pi = buff; pi < buff + p_count; pi++) {

    char* state = "";

    switch (pi->state) {
      case PSLEEPING:
        state = "SLEEPING";
        break;
      case PRUNNABLE:
        state = "RUNNABLE";
        break;
      case PRUNNING:
        state = "RUNNING ";
        break;
      case PZOMBIE:
        state = "ZOMBIE  ";
        break;
    }

    printf("%d\t%s\t%s\t%d\t%s\n",
      pi->pid,
      pi->name,
      state,
      pi->parent_pid,
      pi->parent_name
    );
  }

  free(buff);
  exit(0);
}
