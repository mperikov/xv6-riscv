#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[251];

void assert(int x) {
  if (!x)
    printf("failed\n");
  else
    printf("passed\n");
}

void test_null() {
  int fd = open("/dev/null", O_RDWR);
  int res = write(fd, "smth", 4);

  printf("Test null write: ");
  assert(res == 4);

  res = read(fd, buf, 1);

  printf("Test null read: ");
  assert(res == 0);

  close(fd);
}


void test_zero() {

  int fd = open("/dev/zero", O_RDWR);
  int res = read(fd, buf, 250);

  printf("Test zero write: ");
  res = write(fd, "smth123456", 10);
  assert(res == -1);

  res = read(fd, buf, 250);
  int fl = 1;
  for (int i = 0; i < 251;i++)
    fl &= buf[i] == 0;

  printf("Test zero read: ");
  
  assert((res == 250) && fl);

  close(fd);
}


void test_urandom() {


  int fd = open("/dev/urandom", O_RDWR);
  int res = read(fd, buf, 5);

  printf("Test urandom read wrong size: ");
  assert(res == -1);

  res = read(fd, buf, 4);

  printf("Test urandom read: ");
  assert(res == 4);

  int tmp[4];
  for (int i = 0; i < 4;i++)
    tmp[i] = buf[i];

  uint seed = 30;
  res = write(fd, &seed, sizeof(seed));

  printf("Test urandom read: ");
  assert(res == 4);

  res = read(fd, buf, 4);

  printf("Test urandom write after seed reset: ");
  int fl = 1;
  for (int i = 0; i < 4; i++)
    fl &= tmp[i] == buf[i];
  assert(fl && res == 4);
  
  close(fd);
}

void test_nullstat() {
  int fd = open("/dev/nullstat", O_RDWR);

  int res1 = write(fd, "smth", 4), res2 = write(fd, "123456789", 9);;

  printf("Test nullstat write: ");
  assert(res1 == 4 && res2 == 9);

  uint64 cnt1, cnt2;
  res1 = read(fd, &cnt1, sizeof(cnt1));

  res2 = read(fd, &cnt2, 10);

  printf("Test nullstat read: ");
  assert(res1 == sizeof(cnt1) && cnt1 == 4 + 9 && res2 == -1);

  close(fd);
}

int main(int argc, char** argv) {
  test_null();
  test_zero();
  test_urandom();
  test_nullstat();
}
