CC = mpicc
CFLAGS = -g -Wall -fopenmp

target = KNN
#由于 .c文件在不同的文件下，那么匹配模式 要匹配不同文件下的%.c文件
src1 = $(wildcard ./src/*.c)
lib1 = $(wildcard ./lib/*.h)
# src2 = $(wildcard ./*.c)
obj1 = $(patsubst %.c,%.o,$(src1))
# obj2 = $(patsubst %.c,%.o,$(src2))
src = ./src/
$(target):$(obj1) $(lib1)
	$(CC) $(CFLAGS) $^ -o $@ -lm
#	$(CC) $(obj) -o $(target)
#编译 src下的.c文件 为.o文件	
%.o:$(src)%.c $(lib1)
	$(CC) $(CFLAGS) -c $< -o $@ -lm
#编译 当前文件下的main.c文件
%.o:%.c $(lib1)
	$(CC) $(CFLAGS) -c $< -o $@
.PHONY: clean 
#防止 外面有clean文件 阻止执行clean
clean:
	rm -f $(target) $(obj1)
