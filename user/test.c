#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

void incorrect_address_test() {
  printf("Test incorrect address: ");
  struct procinfo* buff = (void*) - 1;
  if (ps_listinfo(buff, 10) < 0)
    printf("passed");
  else
    printf("failed");
  printf("\n");
}

void insufficient_buffer_size_test() {
  printf("Test insufficient buffer size: ");
  struct procinfo* buff = malloc(sizeof(struct procinfo));
  if (buff == 0) {
    fprintf(2, "malloc failed\n");
    exit(1);
  }
  if (ps_listinfo(buff, 1) > 0)
    printf("passed");
  else
    printf("failed");
  printf("\n");
  free(buff);
}

int
main(int argc, char* argv[])
{
  incorrect_address_test();
  insufficient_buffer_size_test();
  exit(0);
}
