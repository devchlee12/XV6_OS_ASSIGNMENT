#include"types.h"
#include"user.h"
#include"date.h"
int main(int argc,char *argv[])
{
	int		seconds;
	struct	rtcdate r;

	if(argc<=1)
		exit();
	seconds = atoi(argv[1]);//인자를 숫자로 바꿈
	alarm(seconds);//알람 시스템 콜
	date(&r); //date 시스템 콜 호출해서 시간 얻기
	printf(1,"SSU_AlarmStart\n");
	printf(1, "Current time : %d-%d-%d %d:%d:%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);
	while(1)//알람 시스템 콜이 프로세스 종료시킬 때 까지 기다리기
	;
	exit();
}
