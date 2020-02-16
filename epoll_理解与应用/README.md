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

具体服务端代码如下：

```cpp
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define SIZE 50

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in se_adr, cl_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[4];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&se_adr, 0, sizeof(se_adr));
    se_adr.sin_family = AF_INET;
    se_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    se_adr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (struct sockaddr*)&se_adr, sizeof(se_adr)) == -1)
        error("bind() error");
    listen(ser_sock, 5);
    epfd = epoll_create(SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*SIZE);

    event.events = EPOLLIN;
    event.data.fd = ser_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ser_sock, &event);
    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, SIZE, -1);
        if (event_cnt == -1)
            error("wait error");
        puts("wait......");
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == ser_sock) {
                adr_sz = sizeof(cl_adr);
                cli_sock = accept(ser_sock, (struct sockaddr*) &cl_adr, &adr_sz);
                event.events = EPOLLIN;
                event.data.fd = cli_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cli_sock, &event);
                printf("new connected ......\n");
            }
            else {
                str_len = read(ep_events[i].data.fd, buf, SIZE);
                if (str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("disconnect %d\n", ep_events[i].data.fd);
                }
                else {
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(ser_sock);
    close(epfd);
    return 0;
}
```



#### 1. 条件触发与边缘触发

  **边缘触发** 是指每当状态变化时发生一个io事件； 

  **条件触发** 是只要满足条件就发生一个io事件

##### 1.1 条件触发

​	就是在每当收到客户端数据都会注册事件，在输入缓存区还剩的情况下会一直注册。

select 是条件触发

上述的服务端就属于条件触发

##### 1.2 边缘触发

当客户端接收事件时，只注册一次。

*  通过errno变量验证错误原因
* 为完成非阻塞I/o，更改套接字特性

```cpp
#include <fcntl.h>
int fcntl(int filedes, int cmd,.....);
	->成功返回cmd相关参数， 失败返回-1
        filedes 文件描述符
        cmd 表示函数调用的目的
```

修改为非阻塞模式

```cpp
int flag = fcntl(fd, F_GETEL, 0);
fcntl(fd, F_SETFL, flag|O_NONBLOCK);

```

```cpp
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define SIZE 50

void error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void setmode(int fd) {
    int flage = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flage|O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    int ser_sock, cli_sock;
    struct sockaddr_in se_adr, cl_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[4];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    ser_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&se_adr, 0, sizeof(se_adr));
    se_adr.sin_family = AF_INET;
    se_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    se_adr.sin_port = htons(atoi(argv[1]));
    if (bind(ser_sock, (struct sockaddr*)&se_adr, sizeof(se_adr)) == -1)
        error("bind() error");
    listen(ser_sock, 5);
    epfd = epoll_create(SIZE);
    ep_events = malloc(sizeof(struct epoll_event)*SIZE);

    setmode(ser_sock);
    event.events = EPOLLIN;
    event.data.fd = ser_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, ser_sock, &event);
    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, SIZE, -1);
        if (event_cnt == -1)
            error("wait error");
        puts("wait......");
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == ser_sock) {
                adr_sz = sizeof(cl_adr);
                cli_sock = accept(ser_sock, (struct sockaddr*) &cl_adr, &adr_sz);
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = cli_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cli_sock, &event);
                printf("new connected ......\n");
            }
            else {
                while (1) {
                    str_len = read(ep_events[i].data.fd, buf, SIZE);
                    if (str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("disconnect %d\n", ep_events[i].data.fd);
                        break;
                    }
                    else if (str_len < 0){
                        if (errno == EAGAIN)
                            break;
                    }
                    else {
                        write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(ser_sock);
    close(epfd);
    return 0;
}
```

