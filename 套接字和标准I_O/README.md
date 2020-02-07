# 套接字和标准I/O

#### 1. 标准I/O函数的优点

* 具有良好的移植性
*  由于会额外得到一个缓冲区，可以提高性能

#### 2. 标准I/O函数的缺点

* 不易进行双向通信
* 有时会频繁调用fflush函数
* 需要以FILE结构体指针的形式返回文件描述符

```c++
#include <stdio.h>
FILE* fdopen(int fd, "w");
int fileno(FILE *stream);       ///FILE指针和文件描述符的转换

fflush(FILE *stream); ///清除读写缓冲区，在需要立即把输出缓冲区的数据进行物理写入时
```

#### 3. 套接字的标准I/O函数使用

示例：

[echo_stdserv.c](echo_stdserv.c)

[echo_stdclient.c](echo_stdclient.c)

```bash
h1@ubuntu ~/p/t/套接字和标准I_O> ./echo_stdserv 9190 &
h1@ubuntu ~/p/t/套接字和标准I_O> ./echo_stdclient 127.0.0.1 9190
connect ....
INPUT: (q->end) sda 
sda

INPUT: (q->end) 欧耨
欧耨

INPUT: (q->end) 你好
你好

INPUT: (q->end) q
```

外加I/O复用技术

示例：

[echo_stdserv_sel.c](echo_stdserv_sel.c)

