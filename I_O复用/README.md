# I/O复用

​	主要讨论并发服务器的第二种实现方法

#### 1. 基于I/O复用的服务端

##### 1.1 复用

在一个通信频道中传输多个数据。防止同时传输会采用**时分复用技术**。

因数据频率不同即便同时也能进行一定的区分这里利用**频分复用技术**。

所以引入复用后就可以减少进程的使用。

##### 2. 理解select函数并实现服务端

​	windows平台下有相同的函数相同的功能。因此有良好的移植性。

##### 2.1 select函数的功能和调用顺序

​	作用：可以将多个文件描述符集中一起监视。

* 是否纯在套接字接收数据
* 无需阻塞传输数据的套接字有那些
* 那些套接字发生异常

这些都输监视项（事件）

* 调用顺序与方法：

  步骤一：设置文件描述符-》指定监视范围-》设置超时

  步骤二：调用select函数

  步骤三：查看调用结果

##### 2.2 设置文件描述符

​	可以同时监视多个文件描述符。

分为三类监视项集中起来（接收， 传输， 异常）

​	主要使用fd_set数组变量来执行，该数组只存0和1.

数组中数值为1的位置表示该位置的描述符为监视对象。

* FD_ZERO(fd_set *fdset) 将所有fd_set变量初始为0
* FD_SET(int fd, fd_set *fdset)注册文件描述符
* FD_CLR(int fd, fd_set *fdset) 从参数fdset指向的变量中清楚文件描述符fd的信息
* FD_ISSET(int fd, fd_set *fdset)若参数fdset指向的变量中有fd文件描述符信息并且发生事件响应的，返回真。

```c++
#include <sys/select.h>
#include <sys/time.h>

int select(
	int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);
	成功返回大于0的发生事件文件描述符数量值，超时返回0，失败返回-1.
        magxfd 监视对象文件描述符的数目
        readset 将所有关注的是否存在待读取数据的，并传递地址值
        writeset 关注是否传输无阻塞数据，并传递地址值
        exceptset 关注是否发生异常，并传递地址值
        timeout 防止无限阻塞，传递超时（time-out）信息
        struct timeval {
            long tv_sec; //seconds
            long tv_usec; //microseconds
        }
```

所以主要决定的是 文件描述符监视的范围

​									超时时间

文件描述符是从0开始的

文件描述符0是标准输入变化的描述符

函数用法示例：

* [select.c](select.c)

编译运行：

```bash
gcc select.c -o select
./select 
你好
abc
哈哈 ##间隔10秒后输入的
```

运行结果：

```bash
message :
你好

message :
abc
time-out
time-out
message :
哈哈哈

```

如果运行中无任何输入就会发生超时。

* 每次调用select后timeout会变成剩下的时间。所以每次都要初始化。

由上面可实现I/O复用的并发回声服务端

示例：

* [echo_selectserv.c](echo_selectserv.c)

连接时是利用三次握手传输的数据来判断有连接进入，在调用accept()接收数据进行连接并将连接到的套接字加入到监听中。

所以在每次有事件发生时，需判断是否是服务端套接字的，如果是就是有连接接入，事件的其他套接字都属于客户端消息或者断开连接时的四次握手消息。

运行结果：echo_selectserv.c

```bash
./echo_selectserv 9190
server socket :3
client socket : 4
server socket :3
client socket : 5
disconnect client : 5
disconnect client : 4
```

echo_mpclient.c 1

```bash
./echo_mpclient 127.0.0.1 9190
abc
SERVER OUT: abc
q
```

echo_mpclient.c 2

```bash
./echo_mpclient 127.0.0.1 9190
你好
SERVER OUT: 你好
q
```

总结：何为I/O复用 顾名思义多个数据传输一起用一个通道进行传输，利用他们不同的时间或者频率（socket）来区分。

解决了多进程中的一大缺点浪费大量内存和运算。

select需要反复调用来监听，因为它的返回值就是监听到的数量。

windows 的套接字不是从0开始递增的。所以它的fd_set 有两个人成员一个是套接字句柄的数量，另一个是存放套接字的数组。