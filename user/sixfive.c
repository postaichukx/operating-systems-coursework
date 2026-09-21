#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
sixfive(int fd)
{
  char c;
  char buf[64];
  int len = 0;
  int valid = 1;
  const char *delims = " -\r\t\n./,";

  while (read(fd, &c, 1) > 0) {
    if (strchr(delims, c) != 0) {
      if (len > 0 && valid) {
        buf[len] = '\0';
        int num = atoi(buf);
        if (num % 5 == 0 || num % 6 == 0) {
          printf("%d\n", num);
        }
      }
      len = 0;
      valid = 1;
    } else {
      if (c >= '0' && c <= '9') {
        if (len < sizeof(buf) - 1) {
          buf[len++] = c;
        }
      } else {
        valid = 0;
      }
    }
  }

  if (len > 0 && valid) {
    buf[len] = '\0';
    int num = atoi(buf);
    if (num % 5 == 0 || num % 6 == 0) {
      printf("%d\n", num);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  if (argc <= 1) {
    sixfive(0);
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      exit(1);
    }
    sixfive(fd);
    close(fd);
  }

  exit(0);
}