#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data, int len);

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Example 1:\n");
    int a[2] = {61810, 2026};
    memdump("ii", (char *)a, sizeof(a));

    printf("Example 2:\n");
    memdump("S", "a string", sizeof("a string"));

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s, sizeof(s));

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example, sizeof(example));

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example, sizeof(example));
  } else if (argc == 2) {
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data)) {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data, n);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data, int len)
{
  // Your code here.  `data` holds `len` valid bytes.
  int offset = 0;

  for (int i = 0; fmt[i] != '\0'; i++) {
    switch (fmt[i]) {
    case 'c': {
      if (offset + 1 <= len) {
        printf("%c\n", data[offset]);
        offset++;
      } else {
          printf("memdump: not enough data for 'c'\n");
          return;
        }
      break;
    }

    case 'h': {
      short val;
      if (offset + 2 <= len) {
        memmove(&val, data + offset, sizeof(val));
        printf("%d\n", val);
        offset += 2;
      } else {
          printf("memdump: not enough data for 'h'\n");
          return;
      }
      break;
    }

    case 'i': {
      int val;
      if(offset+4<=len){
        memmove(&val, data+offset, sizeof(val));
        printf("%d\n",val);
        offset +=4;
      }
      else{
        printf("memdump: not enough data for 'i'\n");
        return;
      }

      break;
    }

    case 'p': {
      if(offset+8 <= len){
        long int val;
        memmove(&val, data+offset, sizeof(val));
        printf("%lx\n",val);
        offset+=8;
      }
      else{
        printf("memdump: not enough data for 'p'\n");
        return;
      }

      break;
    }

    case 'S': {
      while(offset < len && data[offset] != '\0'){
        printf("%c", data[offset]);
        offset++;
      }
      printf("\n");

      break;
    }

    case 's': {
      char *ptr;
      if(offset+8 <= len){
        memcpy(&ptr, data + offset, sizeof(ptr));
        printf("%s\n", ptr);
      }
      else{
        printf("memdump: not enough data for 's'\n");
        return;
      }
      offset += 8;
      break;
    }

    default: {
      printf("memdump: unknown format '%c'\n", fmt[i]);
      return;
    }
    }
  }

}
