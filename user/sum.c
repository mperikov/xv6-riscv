#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
  const int max = 20;
  int i, cc;
  char c;
  char buf[max];

  buf[max - 1] = '\0';

  for (i = 0; ; i++) {
    cc = read(0, &c, 1);
    if (cc < 0) {
      fprintf(2, "Error while reading\n");
      exit(0);
    }
    if (cc == 0)
      break;
    if (i + 1 < max)
      buf[i] = c;
    if (c == '\n' || c == '\r')
      break;
  }

  buf[i] = '\0';

  if (i + 1 >= max) {
    fprintf(2, "Buffer overflow\n");
    exit(0);
  }

  printf("|%s|\n", buf);


  int pos = 0;
  for (; pos + 1 < max; pos++) {
    if (buf[pos] == ' ')
      break;
  }
  int cnt = 0;
  for (int j = 0; j + 1 < max; j++) {
    if (buf[j] == '\0')
      break;
    if (buf[j] < '0' || buf[j] > '9')
      cnt++;
  }
  if (pos == 0 || buf[pos] != ' ' || pos + 1 == max || buf[pos + 1] == '\0' || cnt != 1) {
    fprintf(2, "Incorrect format\n");
    exit(0);
  }

  int a = atoi(buf);
  int b = atoi(buf + pos + 1);
  printf("%u\n", add(a, b));

  exit(0);
}
