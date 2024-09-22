#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "sysinfo.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_sysinfo(void)
{
  uint64 p;

  argaddr(0, &p);
  struct sysinfo info;
  info.freemem = kfreecnt();
  info.nproc = proccount();

  if(copyout(myproc()->pagetable, p, (char *)&info, sizeof(info)) < 0)
    return -1;
  return 0;
}
