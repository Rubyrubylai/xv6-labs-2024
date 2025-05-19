#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

void
plicinit(void)
{
  // set desired IRQ priorities non-zero (otherwise disabled).
  // 設置 PLIC 會接收哪些中断，進而將中断路由到 CPU
  *(uint32*)(PLIC + UART0_IRQ*4) = 1; // 設置 PLIC 接收來自 UART 的中斷
  *(uint32*)(PLIC + VIRTIO0_IRQ*4) = 1; // 設定 PLIC 接收來自磁碟的中斷
}

// plicinit 是由 0 號 CPU 運行，之后，每個 CPU 的核都需要調用 plicinithart 函数表明對於哪些外設中斷感興趣
void
plicinithart(void)
{
  int hart = cpuid();
  
  // set enable bits for this hart's S-mode
  // for the uart and virtio disk.
  *(uint32*)PLIC_SENABLE(hart) = (1 << UART0_IRQ) | (1 << VIRTIO0_IRQ); // 每個 CPU 的核都表明自己對於 UART 和 VIRTIO 的中斷感興趣

  // set this hart's S-mode priority threshold to 0.
  *(uint32*)PLIC_SPRIORITY(hart) = 0;
}

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
  int hart = cpuid();
  int irq = *(uint32*)PLIC_SCLAIM(hart);
  return irq;
}

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
  int hart = cpuid();
  *(uint32*)PLIC_SCLAIM(hart) = irq;
}
