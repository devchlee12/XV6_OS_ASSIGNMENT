#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

pte_t *walkpgdir(pde_t *pgdir, const void *va, int alloc);

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//가상페이지만 할당하는 시스템 콜
int
sys_ssualloc(void)
{
  int n;
  int addr;
  if (argint(0,&n) < 0){
    return (-1);
  }
  if (n < 0 || n % PGSIZE != 0)
    return (-1);
  addr = myproc()->sz;
  (myproc()->sz) += n; 
  return addr;
}

//가상 메모리 페이지 개수 반환
int
sys_getvp(void)
{
  if (myproc()->sz % PGSIZE == 0)
    return ((myproc()->sz)/PGSIZE);
  else
    return ((myproc()->sz)/PGSIZE + 1);
}
//물리메모리 페이지 개수 반환
int
sys_getpp(void)
{
  pte_t *pte;
  int n = 0;
  for(int i = 0;i<myproc()->sz;i+=PGSIZE){
    pte = walkpgdir(myproc()->pgdir, (void *) i, 1);
    if((*pte & PTE_P))
      n++;
  }
  return (n);
}
