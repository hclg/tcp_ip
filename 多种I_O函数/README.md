# 多种I/O复用

这里将介绍一些不同I/O函数的区别。

#### 1. send & recv 函数

​	跟windows的没有什么特别的差别。

```c++
#include <sys/socket.h>
ssize_t send(int sockfd, const void * buf, size_t nbytes, int flags);
	成功时返回发送的字节数，失败返回-1。
        sockfd 套接字文件描述符
        部分保存待传输数据的缓冲地址值
        待传输的字符数
        传输数据指定可选项信息
ssize_t recv(int sockfd, void* buf, size_t nbytes, int flags);
	成功返回接收字节数（收到EOF时返回0），失败返回-1
        同上类似
```

|    可选项     |                        含义                        | send | recv |
| :-----------: | :------------------------------------------------: | :--: | :--: |
|    MSG_OOB    |                  用于传输带外数据                  |  -   |  -   |
|   MSG_PEEK    |          验证输入缓冲中是否存在接收的数据          |      |  -   |
| MSG_DONTROUTE | 数据传输过程中不参照路由表，在本地网络中寻找目的地 |  -   |      |
| MSG_DONTWAIT  |       调用I/O函数时不阻塞，用于使用非阻塞I/O       |  -   |  -   |
|  MSG_WAITALL  |       防止函数返回,直到接收全部请求的字节数        |      |  -   |

##### 1.1 MSG_OOB 发送紧急消息

​	用于发送创建特殊发送方法和通道以发送紧急消息。

示例：

* [oob_send.c](oob_send.c)
* [oob_serv.c](oob_serv.c)

fcntl(recv_sock, F_SETOWN, getpid());

将文件描述符recv_sock 指向的套接字所有者F_SETOWN 改为getpid返回值ID的进程。

在本系统中输出跟理想的不一样。

所以无法做出评测以后回补。。。

其他慢慢看到在学。