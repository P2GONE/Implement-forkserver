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

### How to Run
./forkserver ./target_program
