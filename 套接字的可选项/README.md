# 套接字的多种可选项

#### 1.  套接字可选项和I/O缓冲大小

我们进行套接字编程时往往只关注数据通信，而忽略了套接字具有的不同特性。但是，理解这些特性并根据实际需要进行更改也很重要.

##### 1.1 套接字的多种可选项

可选项是分层的。

| 协议层      | 选项名            | 读取 | 设置 |
| ----------- | ----------------- | ---- | ---- |
| SOL_SOCKET  | SO_SNDBUF         | O    | O    |
| SOL_SOCKET  | SO_RCVBUF         | O    | O    |
| SOL_SOCKET  | SO_REUSEADDR      | O    | O    |
| SOL_SOCKET  | SO_KEEPALIVE      | O    | O    |
| SOL_SOCKET  | SO_BROADCAST      | O    | O    |
| SOL_SOCKET  | SO_DONTROUTE      | O    | O    |
| SOL_SOCKET  | SO_OOBINLINE      | O    | O    |
| SOL_SOCKET  | SO_ERROR          | O    | X    |
| SOL_SOCKET  | SO_TYPE           | O    | X    |
| IPPROTO_IP  | IP_TOS            | O    | O    |
| IPPROTO_IP  | IP_TTL            | O    | O    |
| IPPROTO_IP  | IP_MULTICAST_TTL  | O    | O    |
| IPPROTO_IP  | IP_MULTICAST_LOOP | O    | O    |
| IPPROTO_IP  | IP_MULTICAST_IF   | O    | O    |
| IPPROTO_TCP | TCP_KEEPALIVE     | O    | O    |
| IPPROTO_TCP | TCP_NODELAY       | O    | O    |
| IPPROTO_TCP | TCP_MAXSEG        | O    | O    |

大致常用的协议层分三类

SOL_SOCKET层：是套接字的相关通用可选项

IPPROTO_IP层：是IP协议相关事项

IPPROTO_TCP层：是TCP协议相关事项

##### 1.2 常用操作

有以下两个操作Get读取 and Set 设置。

读取函数

```c++
#include <sys/socket.h>
int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
成功返回0， 失败返回-1
    sock --> 套接字描述符
    level --> 要查看的协议层
    optname --> 可选项名
    optval --> 保存查看结果的缓冲地址值
    optlen --> 向第四个参数optval 传递的缓冲大小
```

设置函数

```c++
#include <sys/socket.h>
int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
成功0 失败 -1
    optval 保存要修改的选项信息缓冲地址值
    optlen 传递可选项信息的字节数
    其他同上
```

调用getsockopt函数的事例如下：

* [sokc_type](https://github.com/hclg/tcp_ip/blob/master/%E5%A5%97%E6%8E%A5%E5%AD%97%E7%9A%84%E5%8F%AF%E9%80%89%E9%A1%B9/sock_type.c)
* 注意事项: 运行有时候会发生错误，多运行几次即可。
* 套接字类型只能在创建决定，后期不能更改。

编译运行：

```bash
gcc sock_type.c -o sock_type
./sock_type
```

运行结果：

```bash
SOCK_STREAM: 1
 SOCK_DGRAM: 2
PF_INET: 2
TCP_sock type : 1 
 optlen: 4
UDP_sock type : 2 
 optlen: 4
```

##### 1.3 I/O缓冲大小

​	生成套接字的时候将同时生成I/O缓冲。

​	SO_RCVBUF是输入缓冲大小可选项，SO_SNDBUF是输出缓冲大小可选项

读取与更改事例

读取

* [git_buf.c](https://github.com/hclg/tcp_ip/blob/master/%E5%A5%97%E6%8E%A5%E5%AD%97%E7%9A%84%E5%8F%AF%E9%80%89%E9%A1%B9/get_buf.c)

编译运行：

```bash
gcc get_buf.c -o get_buf
./get_buf
```

运行结果：

```bash
INPUT buffer: 131072
OUTPUT buffer: 16384
```

这是我的输入输出缓冲大小

更改

* [set_buf.c](https://github.com/hclg/tcp_ip/blob/master/%E5%A5%97%E6%8E%A5%E5%AD%97%E7%9A%84%E5%8F%AF%E9%80%89%E9%A1%B9/set_buf.c)

编译运行：

```bash
gcc set_buf.c -o set_buf
./set_buf
```

运行结果：

```bash
INPUT : 6144
OUTPUT : 6144
```

运行结果跟预想不一样很正常，因为缓冲大小设置需要很谨慎，不会完全按我们的要求去改，会自动保证一点空间。

