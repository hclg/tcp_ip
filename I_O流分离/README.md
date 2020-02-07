# I/O流分离

​	调用fopen后创建了流，相当于数据收发的路径。

#### 1. 分离I/O流的方法

* 利用fork函数复制文件描述符，分两进程进行I/O
* 通过调用两次fdopen,创建读写两个file指针

#### 2. 文件描述符的复制和半关闭

FILE指针->文件描述符->套接字

只要注销任何一个指针都会自动注销相应的文件描述符

所以只要复制一个文件描述符即可，让每个指针对应一个，就可以实现半关闭了。

但

还需要发送EOF才可以。

##### 2.1复制文件描述符的dup&duo2

```c++
#include <unistd.h>
int dup(int fildes);
int dup2(int fildes, int fildes2);
	->成功时返回复制的文件描述符， 失败返回-1
        fildes 需要复制的文件描述符
        fildes2 明确指定的文件描述符整数值
```

验证：[dup.c](dup.c)

运行结果：

```bash
h1@ubuntu ~/p/t/I_O流分离> ./dup 
fd1 = 3, fd2 = 7 
Hi~
It's nice day~ 
Hi~
```

对与发送EOF只需吧FILE在转回文件描述符即可。

无论复制多少个文件描述符，调用一个shutdown就会发送EOF。

单进程半发送i/o流分离服务端 

* [sep_serv2.c](sep_serv2.c)

