# 프로젝트 설정
CC = gcc                    # 컴파일러
CFLAGS = -Wall -Wextra -g   # 컴파일 옵션 (경고 및 디버깅 활성화)
LDFLAGS =                   # 링크 옵션
TARGET = forkserver         # 출력 실행 파일 이름

# 소스 및 헤더 파일
SRCS = main.c forkserver.c shared_memory.c timeout.c
target_SRCS = target.c
HEADERS = forkserver.h shared_memory.h timeout.h

# 오브젝트 파일 (SRCS에서 .c를 .o로 변환)
OBJS = $(SRCS:.c=.o)
target_OBJS = $(target_SRCS:.c=.o)

# 기본 빌드 타겟
all: $(TARGET) target

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# target 실행 파일 생성
target: $(target_OBJS)
	$(CC) $(CFLAGS) $(target_OBJS) -o target $(LDFLAGS)

# 개별 오브젝트 파일 생성 규칙
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 클린 타겟
clean:
	rm -f $(OBJS) $(target_OBJS) $(TARGET) target

# 리빌드 타겟
rebuild: clean all
