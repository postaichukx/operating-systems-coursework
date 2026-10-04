#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: sleep <ticks>\n");
    exit(1);
  }

  int ticks = atoi(argv[1]);
  printf("na cvilu(%d tikov) sa pozastavi system\n", ticks);
  pause(ticks);

  exit(0);
}
