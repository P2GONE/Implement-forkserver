## Custom Forkserver!

```
forkserver_project/
├── Makefile               // 빌드 설정 파일
├── main.c                 // 메인 실행 파일
├── forkserver.h           // fork server 헤더
├── forkserver.c           // fork server 구현
├── shared_memory.h        // 공유 메모리 헤더
├── shared_memory.c        // 공유 메모리 구현
├── timeout.h              // 타임아웃 설정 헤더
├── timeout.c              // 타임아웃 설정 구현
```
### Build
```
make
```
### How to Run
./forkserver ./target_program

### RUN
```
Forkserver initialized.
[Parent] Created child process. Parent PID: 2436, Child PID: 2438
[Child] Process created successfully! PID: 2438
HI, this is test programCould You exploit my program?[Parent] Created child process. Parent PID: 2437, Child PID: 2439
[Child] Process created successfully! PID: 2439
HI, this is test programCould You exploit my program?
AAAAAAAAA
[Parent] Child exited with status: 1
AAAAAAAAAAAAAAAAA
[Parent] Child exited with status: 1
[Parent] Created child process. Parent PID: 2436, Child PID: 2440
[Child] Process created successfully! PID: 2440
HI, this is test programCould You exploit my program?[Parent] Created child process. Parent PID: 2437, Child PID: 2441
[Child] Process created successfully! PID: 2441
HI, this is test programCould You exploit my program?
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
[Parent] Child exited with status: 1
B=[Child] Process created successfully! PID: 2442
[Parent] Created child process. Parent PID: 2437, Child PID: 2442
HI, this is test programCould You exploit my program
[Parent] Child exited with status: 1
AAAAAAAAAAAAAAAAAAA
[Parent] Child exited with status: 1
[Parent] Created child process. Parent PID: 2436, Child PID: 2443
[Child] Process created successfully! PID: 2443
HI, this is test programCould You exploit my program?[Parent] Created child process. Parent PID: 2437, Child PID: 2444
[Child] Process created successfully! PID: 2444
HI, this is test programCould You exploit my program?
DDDDDDDDDDD
[Parent] Child exited with status: 1
[Parent] Created child process. Parent PID: 2437, Child PID: 2445
[Child] Process created successfully! PID: 2445
HI, this is test programCould You exploit my program?
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
[Parent] Child exited with status: 1
[Parent] Created child process. Parent PID: 2436, Child PID: 2446
[Child] Process created successfully! PID: 2446
```
