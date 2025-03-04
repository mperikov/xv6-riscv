#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void write_buf(int fd, char* buf, int size) {
  if (write(fd, buf, size) == -1) {
    fprintf(2, "write error\n");
    exit(1);
  }
}

int
main(int argc, char* argv[])
{
  int pipefd[2];

  pipe(pipefd);

  int pid = fork();
  if (pid == 0) {
    close(0);
    dup(pipefd[0]);
    close(pipefd[0]);
    close(pipefd[1]);

    char* argv[2];
    argv[0] = "wc";
    argv[1] = 0;
    exec("/wc", argv);
  }
  else if (pid > 0) {
    close(pipefd[0]);

    // Вывод блоками по 20 байтов
    char buf[20];
    int k = 0;
    for (int i = 1; i < argc; i++) {

      for (int j = 0; argv[i][j] != 0; j++) {
        buf[k++] = argv[i][j];
        if (k == 20) {
          k = 0;
          write_buf(pipefd[1], buf, 20);
        }
      }
      buf[k++] = '\n';
      if (k == 20) {
        k = 0;
        write_buf(pipefd[1], buf, 20);
      }
    }
    if (k != 0) {
      write_buf(pipefd[1], buf, k);
    }
    //

    close(pipefd[1]);

    int exit_code;
    wait(&exit_code);
    if (exit_code != 0) {
      fprintf(2, "error\n");
      exit(1);
    }
  }
  else {
    fprintf(2, "fork error\n");
    exit(1);
  }
  exit(0);
}
