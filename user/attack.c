#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    exit(1);
  }

  char *mem = sbrk(PGSIZE * 32);

  for (int i = 0; i < PGSIZE * 32; i++) {
    if (memcmp(mem + i, "my very very very secret pw is:   ", 34) == 0) {
      write(2, mem + i + 32, 8); // TODO: 不知道為什麼印不出來
      write(2, "\n", 1);
      exit(0);
    }
  }

  write(2, "secret not found\n", 18);
  exit(1);
}
