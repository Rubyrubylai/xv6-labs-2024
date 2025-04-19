#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  nargv[argc-2] = 0;
  exec(nargv[0], nargv);
  printf("trace: exec failed\n");
  exit(0);
}

// user/trace.c -> 呼叫 trace(32)
//        ↓
// user/user.h -> 宣告 int trace(int)
//        ↓
// user/usys.pl -> 加入 entry("trace")，Makefile 自動產生 user/usys.S
//        ↓
// user/usys.S -> 組語 stub，呼叫 ecall
//        ↓
// kernel/syscall.h -> 加入 SYS_trace = XX
//        ↓
// kernel/syscall.c -> 加入對應 case 和 sys_trace()
//        ↓
// kernel/sysproc.c -> 實作 uint64 sys_trace()
