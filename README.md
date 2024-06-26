# OS과제

### file_system_lazy_init
- Memory Allocation and Multi-level File System
- 가상 메모리 할당을 위한 시스템 콜 구현 및 메모리 정보를 확인하기 위한 ssualloc() getvp(), getpp() 시스템 콜 구현
- multi-level 파일시스템 구현 (lazy init)

### scheduler
- xv6의 기존 스케줄러를 변경.
  - FreeBSD 5.4 이후 구현된 ULE(non-interactive) 스케쥴러 기반의 커스텀 스케쥴러를 구현.
  - 프로세스의 우선순위 + 프로세스가 사용한 시간을 스케줄링에 반영
- 스케줄링 테스트를 할 수 있는 scheduler_test.c 프로그램 및 초기 priority 값을 설정할 수 있는 set_sche_info() 시스템 콜 추가

### date_alarm_system_call
- alarm 시스템 콜 구현

### htac
- 간단한 xv6 내부 프로그램 제작
- 주어진 파일의 맨 밑부터 n개의 줄을 읽고 출력하는 프로그램
