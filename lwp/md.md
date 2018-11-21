> 线程 (轻量级进程) light weight process
- 查看线程 ps -eLf
- 查看线程组 ps -Lw pid

> 线程原语
- pthread_create
创建线程
- pthread_self
获取线程ID
- pthread_join
等待回收线程资源
- pthread_detach
设置线程处于分离态，自动回收资源
- pthread_cancle
进程内取消某个线程
- pthread_exit
退出线程，不会退出进程
- pthread_equal
判断线程相等

> 线程属性
- pthread_attr_init
初始化属性
- pthread_attr_setdetachstate
- pthread_attr_getdetachstate
设置/获取线程分离态
- pthread_attr_setstack
- pthread_attr_getstack
设置/获取线程栈

> 线程同步
## 互斥量 mutex
- pthread_mutex_t
- pthread_mutex_init
- pthread_mutex_destroy
- pthread_mutex_trylock
- pthread_mutex_unlock