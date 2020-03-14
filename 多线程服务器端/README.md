# 多线程服务器端

### 1. 线程

* 优点：
  * 减小开销
  * 数据共享
  * 切换线程方便

#### 1.1 进程与线程的差异

| 进程                             | 线程                           |
| -------------------------------- | ------------------------------ |
| 独立数据，堆，栈区               | 共享数据，堆区域，保持栈区隔开 |
| 在操作系统构成的单独执行流的单位 | 在进程内构成单独执行流的单位   |
| 开销大                           | 开销小                         |
|                                  |                                |

#### 1.2 线程创建及执行

* 单独定义线程的main函数

  ```cpp
  #include <pthread.h>
  
  int pthread_create(
  	pthread_t * restrict thread, const pthread_attr_t * restrict attr,
      void *(*start_routine)(void *), void *restrict arg
  );
  thread 线程id的变量地址值
  attr 传递线程属性参数
  start_routine 线程的main函数
  arg 传递的参数信息
  ```

  

  