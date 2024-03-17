# include "types.h"
# include "user.h"
# include "date.h"

int
main (int argc, char *argv[])
{
	struct rtcdate r;
	if (date(&r)) { // date 시스템 콜 호출하여 r에 시간 정보 넣기
		printf(2, "date failed\n");
		exit();
	}

	printf(1, "Current time : %d-%d-%d %d:%d:%d\n", r.year, r.month, r.day, r.hour, r.minute, r.second);

	exit();
}
