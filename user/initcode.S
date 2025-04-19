# Initial process that execs /init.
# This code runs in user space.

#include "syscall.h"

# exec(init, argv)
.globl start
start:
        la a0, init
        la a1, argv
        li a7, SYS_exec # 將系統呼叫編號放入 a7，a7 是用來存放 syscall 號碼的暫存器（根據 syscall.h，#define SYS_exec 7）
        ecall # 讓 CPU 產生 trap，跳入 kernel 模式。kernel 會讀取 a7 的值，判斷使用者要執行哪個 syscall（如 exec）

# for(;;) exit();
exit:
        li a7, SYS_exit
        ecall
        jal exit

# char init[] = "/init\0";
init:
  .string "/init\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
  .quad init
  .quad 0
