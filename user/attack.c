#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"
#define PGSIZE 4096
#define DATASIZE (64 * PGSIZE)
#define MSG "Here it is: "

int
main(int argc, char *argv[])
{
  char* data = sbrk(DATASIZE);
  if ( data == (char *)-1 ) { exit(1); }
  for ( int i = 0; i <= DATASIZE - strlen(MSG); i++ ){
    if ( memcmp(data + i, MSG, strlen(MSG)) == 0 ){
      char* start = data + i + strlen(MSG);

      if( (start[0] >= 'A' && start[0] <= 'Z') ||
          (start[0] >= 'a' && start[0] <= 'z') ||
          (start[0] >= '0' && start[0] <= '9') ) {
        int page_offset = 32 + (unsigned char)start[0];
        uint64 offset = (uint64)(data + i) % PGSIZE;
        if (offset == page_offset){ 
	  for ( int j = 1; (start + j) < (data + DATASIZE); j++){
            if( start[j] == '\0' ) {                                                       
              write(1, start, j );
              printf("\n");
              exit(0);
            }
          }
        }
      }
    }
  }
  exit(1);
}
