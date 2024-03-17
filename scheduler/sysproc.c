#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

struct runqueue{
  struct proc *head;
  struct proc *tail;
};

extern int scheduler_test_on;
extern int min_priority;
extern struct runqueue runqueue[NRUNQ];

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

int
sys_date(void)
{
	struct rtcdate *date;

	if (argptr(0, (void*)&date, sizeof(struct rtcdate*)) < 0) //argptr로 시스템 콜의 인자를 fetch
		return -1;
	
	cmostime(date); //cmostime을 통해 시간 정보 date에 저장
	return 0;
}

int
sys_alarm(void)
{
	int alarm_time;
  struct rtcdate r;
	if (argint(0, &alarm_time) < 0) //argint로 시스템 콜 인자를 받아옴
		return -1;
	if (alarm_time <= 0){ // 인자로 온 초가 0보다 작거나 같으면 바로 종료시킴
		cmostime(&r);
		cprintf("SSU_Alarm!\n");
		cprintf("Current time : %d-%d-%d %d:%d:%d\n",r.year,r.month,r.day,r.hour,r.minute,r.second);
    kill(myproc() -> pid);
  }
	myproc() -> alarm_timer = alarm_time * 100 + myproc()->alarmticks;//alarm_timer에 alarmticks + 인자 *100을 저장하여 인자(초) 만큼 지났을 때 프로세스 종료하게 함
	return 0;
}

int
sys_set_sche_info(void)
{
  int init_priority;
  int end_tick;
  
  cprintf("set_sche_info() pid = %d\n",myproc()->pid);
  if (argint(0, &init_priority) < 0) //argint로 시스템 콜 인자를 받아옴
		return -1;
  if (argint(1, &end_tick) < 0) //argint로 시스템 콜 인자를 받아옴
		return -1;
  myproc()->priority = init_priority;//우선순위 초기화
  if (min_priority > init_priority)
    min_priority = init_priority;//최소우선순위 초기화
  myproc()->end_cpu_tick = end_tick;
  myproc()->is_end_with_tick = 1;
  
  return 0;
}

int
sys_sche_test_start(void)
{
  scheduler_test_on = 1;
  return 0;
}

int
sys_sche_test_end(void)
{
  scheduler_test_on = 0;
  return 0;
}
