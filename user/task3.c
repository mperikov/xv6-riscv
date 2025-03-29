#include "kernel/types.h"
#include "user/user.h"

int test_write() {
  int mtx;
  if (mutex(&mtx) < 0)
    return -1;

  if (write(mtx, "mutex", 5) >= 0)
    return -1;

  if (close(mtx) <0)
    return -1;

  return 0;
}

int test_read() {
  int mtx;
  if (mutex(&mtx) < 0)
    return -1;

  char buff[5];
  if (read(mtx, buff, 5) >= 0)
    return -1;

  if (close(mtx) < 0)
    return -1;

  return 0;
}

int test_close_locked() {
  int mtx;
  if (mutex(&mtx) < 0)
    return -1;

  if (mutex_lock(mtx) < 0)
    return -1;

  if (close(mtx) < 0)
    return -1;
  return 0;
}

int test_close_locked_other_proc() {
  int mtx;
  if (mutex(&mtx) < 0)
    return -1;

  if (mutex_lock(mtx) < 0)
    return -1;

  int pid = fork();
  if (pid == 0) {
    if (close(mtx) < 0)
      exit(1);
    exit(0);
  }
  else if (pid > 0) {
    int exit_code;
    wait(&exit_code);
    if (exit_code != 0)
      return -1;
  }
  else
    return -1;

  return 0;
}

int test_unlock_other_proc() {
  int mtx;
  if (mutex(&mtx) < 0)
    return -1;

  if (mutex_lock(mtx) < 0)
    return -1;

  int pid = fork();
  if (pid == 0) {
    if (mutex_unlock(mtx) == 0)
      exit(1);
    exit(0);
  }
  else if (pid > 0) {
    int exit_code;
    wait(&exit_code);
    if (exit_code != 0)
      return -1;
  }
  else
    return -1;

  return 0;
}

int main(int argc, char* argv[]) {

  if (test_write() < 0)
    printf("test_wirte failed\n");
  else
    printf("test_wirte passed\n");

  if (test_read() < 0)
    printf("test_read failed\n");
  else
    printf("test_read passed\n");

  if (test_close_locked() < 0)
    printf("test_close_locked failed\n");
  else
    printf("test_close_locked passed\n");

  if (test_close_locked_other_proc() < 0)
    printf("test_close_locked_other_proc failed\n");
  else
    printf("test_close_locked_other_proc passed\n");

  if (test_unlock_other_proc() < 0)
    printf("test_unlock_other_proc failed\n");
  else
    printf("test_unlock_other_proc passed\n");

  exit(0);
}
