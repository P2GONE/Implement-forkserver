## ABOUT Implemented forkserver
### Main Roop
1. 표준 입력으로부터 테스트케이스 입력을 대기함. 사용자가 테스트 케이스를 입력하면 이를 받아 처리함.
2. 입력받은 테스트 케이스를 사용하여 타겟 프로그램을 실행함.
3. 타겟 프로그램의 실행 결과를 사용자에게 출력함
### run_target
1. fork를 사용하여 새로운 자식 프로세스를 생성
2. 자식 프로세스는 execl()을 사용하여 타겟 프로그램을 실행하고, 입력된 테스트 케이스를 인자로 전달함.
3. 부모 프로세스는 자식 프로세스의 종료 상태를 확인하여 크래시 여부를 판단함.
### crash_detection
1.  WIFSIGNALED 매크로를 사용하여 자식 프로세스가 신호에 의해 종료되었는지 확인함.
2.  SIGSEGV, SIGABRT와 같은 신호는 크래시로 간주함.


## RUN
```
Forkserver started. Waiting for testcases...
Enter testcase (or 'exit' to quit): hello
Received input: hello
Testcase executed successfully.
Testcase executed successfully.
Enter testcase (or 'exit' to quit): crash
Crashing as per input...
Testcase caused a crash!
Testcase executed successfully.
Enter testcase (or 'exit' to quit): exit
Exiting forkserver.
```
