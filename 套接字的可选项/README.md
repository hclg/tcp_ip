# 套接字的多种可选项

#### 1.  套接字可选项和I/O缓冲大小



##### 1.1 套接字的多种可选项

可选项是分层的。

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

![image-20200201215833438](/home/h1/.config/Typora/typora-user-images/image-20200201215833438.png)

##### 1.3 I/O缓冲大小

​	生成套接字的时候将同时生成I/O缓冲。

​	SO_RCVBUF是输入缓冲大小可选项，SO_SNDBUF是输出缓冲大小可选项

读取与更改事例

读取

* 

![image-20200201230647194](/home/h1/.config/Typora/typora-user-images/image-20200201230647194.png)