#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"


int
main(int argc, char *argv[])
{
  if(argc != 2){
    printf("Usage: secret the-secret\n");
    exit(1);
  }
  char *end = sbrk(PGSIZE*32); // 請求了 32 頁的空間，回傳值是呼叫 sbrk() 之前的 heap 結尾，是你可以開始使用的記憶體的起始位址
  end = end + 9 * PGSIZE;
  strcpy(end, "my very very very secret pw is:   ");
  strcpy(end+32, argv[1]); // secret 是寫在 sbrk() 回傳的位址 + 9 頁 + 32 bytes
  exit(0);
}

