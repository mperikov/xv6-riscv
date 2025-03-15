#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  int pid = fork();

  if (pid == 0) {
    sleep(100);
    exit(1);
  }
  else if (pid > 0) {
    int exit_code;
    int cur_pid = getpid();

    printf("parent %d\nchild %d\n", cur_pid, pid);

    if (kill(pid) != 0) {
      fprintf(2, "kill error\n");
      exit(1);
    }
    pid = wait(&exit_code);

    printf("child %d\nexit code %d\n", pid, exit_code);
  }
  else {
    printf("fork error\n");
  }
  exit(0);
}
