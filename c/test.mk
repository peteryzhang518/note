hello:main.o test.o
目标：依赖
    gcc -o hello main.o test.o
生成目标的命令



main.o:main.c
依赖：依赖的依赖
    gcc -c main.c -o main.o
生成依赖的命令


test.o:test.c
依赖：依赖的依赖
    gcc -c test.c -o test.o
生成依赖的命令


https://blog.csdn.net/weixin_51609435/article/details/125893962
