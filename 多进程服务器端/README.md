# 多进程服务器端

#### 1. 进程的概念及应用

##### 1.1 并发服务端

​	同时向所有发送请求的客户端提供服务

* 多进程服务器：通过创建多个进程提供服务。
* 多路复用服务器： 通过捆绑并统一管理I/O对象提供服务
* 多线程服务器：通过生成与客户端等量的线程提供服务。

##### 1.2 进程

​	定义：占用内存空间的正在运行的程序。

##### 1.2.1 进程ID

所有进程都会从操作系统分配到一个ID就是进程ID。

其值为大于2的整数，1为操作系统启动后，辅助操作系统的首个进程。

查看进程：

```bash
ps au 				//ps指令可以查看所有进程 a和u参数列出所有进程的详细信息。
```

结果类似：

```bash
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root       1001  0.0  0.0  23284  1924 tty1     Ss+  13:36   0:00 /sbin/agetty -
root       1039  3.4  2.2 529500 91924 tty7     Ssl+ 13:36   0:58 /usr/lib/xorg/
```

##### 1.2.2 通过fork创建进程

​	创建进程的方法有很多，其中fork函数可以在创建多进程服务器中使用。

```c++
#include <unistd.h>
pid_t fork(void);
	成功返回进程ID， 失败返回 -1.
```

​	fork函数是用于复制正在运行的，调用fork函数的进程。

复制相同的内存空间的子进程，可以用fork函数返回值来区分。

* 父进程：fork函数返回子进程的ID（原进程）
* 子进程：fork函数返回0（复制出来的进程）

通过以下代码验证：

* [fork.c](fork.c)

编译运行：

```bash
gcc fork.c -o fork
./fork
```

运行结果：

```bash
parent proc: [9 23] 
Child proc: [13 27] 
```

#### 2. 进程和僵尸进程

​	和文件一样进程也有创建和销毁。如果未销毁进程它就会变成僵尸进程。

##### 2.1 僵尸进程

​	进程完成工作后因被销毁，但有时这些进程会变成僵尸进程，占用系统中的重要资源。这种状态下的进程叫做僵尸进程。

##### 2.2 产生僵尸进程的原因

 终止进程的方法有：

* 传递参数值并调用exit
* main海曙中执行return 并返回值

在用以上方法传递值或返回值都会传递给操作系统。而操作系统会在这些值传递给产生子进程的父进程之前，都不会销毁子进程。而此时状态下的进程就是僵尸进程。

​	通俗一点就是子进程已经执行完调用return了父进程还没结束就不会销毁子进程。

子进程终止会将放回值或exit的参数传给父进程。

验证如下：

* [zombie.c](zombie.c)

编译运行：

```bash
gcc zombie.c -o zombie
./zombie
ps au //30秒内新窗口
ps au //30秒后
```

运行结果：

```bash
child process ID: 15496
Hi, I am a child processEND child process
END parent process //30秒后出现
### 新窗口30内
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root       1001  0.0  0.0  23284  1924 tty1     Ss+  13:36   0:00 /sbin/agetty -
root       1039  3.6  2.5 550280 103008 tty7    Ssl+ 13:36   2:48 /usr/lib/xorg/
h1         2477  0.0  0.1  34076  5236 pts/0    Ss   13:44   0:00 bash
h1         2488  0.0  0.3 187996 13372 pts/0    S    13:44   0:00 fish
h1        15495  0.0  0.0  10824   892 pts/0    S+   14:53   0:00 ./zombie
h1        15496  0.0  0.0      0     0 pts/0    Z+   14:53   0:00 [zombie] <defu
h1        15499  0.2  0.1  34076  5280 pts/1    Ss   14:53   0:00 bash
h1        15510  0.0  0.0  48856  3664 pts/1    R+   14:53   0:00 ps au
### 30秒后
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root       1001  0.0  0.0  23284  1924 tty1     Ss+  13:36   0:00 /sbin/agetty -
root       1039  3.6  2.5 550280 103012 tty7    Ssl+ 13:36   2:50 /usr/lib/xorg/
h1         2477  0.0  0.1  34076  5236 pts/0    Ss   13:44   0:00 bash
h1         2488  0.0  0.3 187996 13372 pts/0    S+   13:44   0:00 fish
h1        15499  0.0  0.1  34076  5304 pts/1    Ss   14:53   0:00 bash
h1        15514  0.0  0.0  48856  3592 pts/1    R+   14:53   0:00 ps au
```

其中 STAT栏目下的Z+表示僵尸进程，比ID少1的是父进程。

提示：

```bash
./zombie &  ## 可以后台运行
```

##### 2.3 销毁僵尸进程

​	销毁僵尸子进程的方法有两种。

##### 2.3.1 利用wait函数

```c++
#include <sys/wait.h>
pid_t wait(int *status);
	成功返回被终止的子进程ID，失败返回-1.
```

调用此函数的个数和子进程个数相同，如果子进程已经正常终止，返回值或参数会保存在该函数的参数中。

* WIFEXITED(status) 子进程正常返回真
* WEXITSTATUS(status) 返回子进程的返回值

示例：

* [wait.c](wait.c)

编译运行：

```bash
gcc wait.c -o wait
./wait &
ps au
```

运行结果：

```bash
child process one ID: 25410
child process two1 ID: 25411
child sen one: 3
child process two2 ID: 25937
child sen two: 7
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root       1001  0.0  0.0  23284  1924 tty1     Ss+  13:36   0:00 /sbin/agetty -
root       1039  4.0  3.1 602460 124376 tty7    Ssl+ 13:36   5:58 /usr/lib/xorg/
h1         2477  0.0  0.1  34076  5236 pts/0    Ss   13:44   0:00 bash
h1         2488  0.0  0.3 187996 13388 pts/0    S    13:44   0:01 fish
h1        15499  0.0  0.1  34076  5304 pts/1    Ss   14:53   0:00 bash
h1        17579  0.0  0.2 187200  8048 pts/1    S+   15:00   0:00 fish
h1        25409  0.0  0.0  10824   880 pts/0    S    16:03   0:00 ./wait
h1        25424  0.0  0.0  48856  3700 pts/0    R+   16:03   0:00 ps au
Job 1, './wait &' has ended
```

这时候就没有僵尸进程了同时也可以看到25409父进程正在运行。

* 注意：如果进程中没有子进程了，调用wait函数程序就会堵塞直到有子进程为止。

