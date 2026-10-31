#include "kernel/types.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define DATASIZE (64 * PGSIZE)
#define MSG "Here it is: "

int
main(int argc, char *argv[])
{
  char *data = sbrk(DATASIZE);
  int marker_len = sizeof(MSG) - 1;

  if (data == (char *)-1)
    exit(1);

  for (int i = 0; i < DATASIZE - marker_len; i++) {
    if (memcmp(data + i, MSG, marker_len) != 0)
      continue;

    char *secret = data + i + marker_len;
    unsigned char first = secret[0];
    if (!((first >= 'A' && first <= 'Z') ||
          (first >= 'a' && first <= 'z') ||
          (first >= '0' && first <= '9')))
      continue;

    // secret.c places the marker at an offset derived from the first byte.
    if ((uint64)(data + i) % PGSIZE != 32 + first)
      continue;

    for (int j = 0; secret + j < data + DATASIZE; j++) {
      if (secret[j] == 0) {
        write(1, secret, j);
        write(1, "\n", 1);
        exit(0);
      }
    }
  }

  exit(1);
}
