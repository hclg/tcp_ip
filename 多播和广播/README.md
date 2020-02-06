# 多播和广播

#### 1. 多播

​	多播方式的数据传输是基于UDP完成的。因此，与UDP服务器/客户端的实现方式非常接近。也就是同时跟多个主机传输数据。

##### 1.1 优点

* 多播服务器端针对特定多播组，只发送1次数据。

* 即使只发送一次数据，但该组的所有客户端都会接收数据

* 多播组数可在IP地址范围内任意添加。

* 加入特定组的即可接收发往多播组的数据。

  多播组是D类IP地址（224.0.0.0~239.255.255.255），向网络传输一个多播数据包时，路由器将其复制并传递给多个主机

##### 1.2路由和TTL 以及加入组的方法

* TTL是决定数据包传输距离的因素每经过一个路由器就减一，当变为0是数据包只能销毁不能在往后传输。TTL的值过大将影响网络流量。
* 设置方法在于协议层IPPROTO_IP,选项名为IP_MULTICAST_TTL,可利用setsockopt（）来设置。



```c++
int send_sock;
int time_live = 64;
...
    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	setsockopt(send_sock,IPPROTO_IP, IP_MULTICAST_LLT, (void*) &time_live, sizeof(time_live));
.....
```

* 加入多播组也是设置可选项。
* 协议层IPPROTO_IP 可选项IP_ADD_MEMVERSHIP;

```c++
int recv_sock;
struct ip_mreq join_adr;
...
    recv_sock =  socket(PF_INET, SOCK_DGRAM, 0);
....
    join_adr.imr_multiaddr.s_addr = "多播组地址信息";
	join_adr.imr_interface.s_addr="加入多播组的主机地址信息";
setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMVERSHIP, (void*) &join_adr, sizeof(join_adr));
....
    其中 
    struct ip_mreq {
        struct in_addr imr_multiaddr; ///写入加入组的IP
        struct in_addr imr_interface; ///写入加入该组的主机IP也可以写INADDR_ANY
    }
组IP只能是D类地址
    recvfrom中的后两个参数，如果不知道目标主机地址可以写NULL和0；
```

示例：

[news_sender.c](news_sender.c)

[news_receiver.c](news_receiver.c)

#### 2. 广播

​	广播分为两种：

* 直接广播
* 本地广播

是于多播相同，也是由UDP完成的，相同一网络的所有主机传输的方法。

上面两种区别主要在IP地址上。

直接广播除了网络地址外其余地址设置为1.例如向网络地址192.12.34中的所有主机传输数据时，可以向192.12.34.255传输。

反之，本地广播中使用的IP地址限定为255.255.255.255 例如，192.32.24网络中的主机向255.255.255.255传输数据时，数据将传递到192.32.24网络的所有主机

示例：

* [news_sender_brd.c](news_sender_brd.c)
* [news_receiver_brd.c](news_receiver_brd.c)

