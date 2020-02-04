# 进程间的通信

基本理解：就是让进程A中的变量状态传给进程B，进程B通过该变量就相当于听到A的通信。

​	进程是完全独立的内存结构，不会共享内存空间。

而是通过创建管道进行通信的，所以管道并非进程内的资源， 而是和套接字一样，属于操作系统（所以也是fork函数无法复制的对象）。通过操作系统提供的内存空间进行通信。

创建管道的函数

```c++
#include <unistd.h>
int pipe(int filedes[2]);
	成功返回0， 失败返回-1。
        filedes[0] 通过管道进行接收数据使用的文件描述符，即为管道出口
        filedes[1] 同过管道进行传输数据使用的文件描述符，即为管道入口。
```

示例：

* [pipe.c](pipe.c)

编译运行：

```bash
gcc pipe.c -o pipe
./pipe
```

运行结果:

```bash
WHO ARE YOU!!
```

* 此管道是双向的

  父进程的fds[1]->子进程的fds[0]

  子进程的fds[1]->父进程的fds[0]

示例:

* [pipe2.c](pipe2.c)

编译运行：

```bash
gcc pipe2.c -o pipe2
./pipe2
```

运行结果:

```bash
parent process output:WHO ARE YOU!!
child process output:IS YOUR FATHER
```

* 但要注意的是管道中的数据谁都可以拿，子进程的输出的数据如果父进程没拿到子进程自家拿了也是可以，但这样的话父进程就会一直等待管道的数据。
* 所以要执行双向还是两个管道比较好。

示例：

* [pipe3.c](pipe3.c)

编译运行：

```bash
gcc pipe3.c -o pipe3
./pipe3
```

运行结果:

```bash
parent process output:WHO ARE YOU!!
child process output:IS YOUR FATHER
```

