#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 3

void
scheduler_func(void){
    sche_test_start();//전역변수 scheduler_test_on을 1로바꿈 이게 1이면 프로세스 정보 출력됨
    printf(1,"start scheduler_test\n");
    for(int i = 0;i<PNUM;i++){
        int pid = fork();
        if(pid == 0){
            if(i == 0){
                set_sche_info(1,110);
            }
            else if(i == 1){
                set_sche_info(22,200);
            }
            else if(i == 2){
                set_sche_info(11,250);
            }
            while(1){
                ;
            }
        }
    }
    for(int i = 0;i<PNUM;i++)
        wait();
    printf(1,"end of scheduler_test\n");
    sche_test_end();//전역변수 scheduler_test_on을 1로바꿈
}

int
main(void)
{
    scheduler_func();
    exit();
}