# epoll 的理解与应用

select 缺点：

* 效率太低
* 每次向操作系统传输对象信息

epoll的优点：

* 无编写针对文件描述符的循环语句
* 无需每次传递监控对象信息

```cpp
#include <sys/epoll.h>
int epoll_create(int size) 创建保存epoll文件描述符的空间
    size 实例大小 可忽略
    创建于套接字相同的资源，也是文件描述符返回
       int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 等待发生变化
    失败返回 -1
   int epoll_ctl(int epfd, int op, int fd, struct epoll_event * event): 向空间注册并注销文件描述符
            成功0 失败-1
    epfd epoll 例程的文件描述符
            op 指定对象的操作
           	fd 对象的文件描述符
            event 事件类型
struct epoll_event {
    __uint32_t events; //发生的事件情况
    epoll_data_t data;
}
typedef union epoll_data {
    void *ptr;
    int fd;
    __unint32_t u32;
    __unint64_t u64;
}epoll_dada_t;

```

参数：

EPOLL_CTL_ADD 注册

。。。。。_DEL 删除

。。。。。_MOD 更改

