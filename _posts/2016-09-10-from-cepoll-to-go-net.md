---
layout: post
title: Guru：从C语言epoll编程到go net实现分析
categories:  Go
tags:  Go， network， epoll
---  

## 说明 

1. go源码版本：1.7 
2. go源码运行环境：Linux 

## epoll在c语言编程示例 

先看一下大家比较熟悉的epoll在c语言中应用，代码取自[rtmpserver_demo](https://github.com/myself659/rtmpserver_demo)中的文件[rtmpepollsrv.c](https://github.com/myself659/rtmpserver_demo/blob/master/rtmpepollsrv.c)

{% highlight c linenos %}  
int RtmpSessionHandle(int iFd, int iEvent, void *pContext)
{
    int iRet;
    RTMP_SESSION *pSession = (RTMP_SESSION *)pContext;
    
    if(iEvent&EPOLLIN )
    {
        if(0 == pSession->handshake)
        {
            iRet = RtmpSessionHandshake(pSession);      
            if(0 != iRet)
            {
                RtmpSessionHandleFin(pSession); 
            }
        }
        else
        {   
            iRet =  RtmpPktHandle(pSession);
        }
    }
    if(iEvent & (EPOLLERR |EPOLLHUP) )
    {
        RtmpSessionHandleFin(pSession);
    }
        
    return iRet;
}




int ListenHandle(int iFd, int iEvent, void *pContext)
{
    int iNewFd;
    int iRet = 0;
    struct sockaddr tmpAddr;
    memset(&tmpAddr, 0, sizeof(tmpAddr));
    int iSocketSize = sizeof(tmpAddr);
    EPOLL_CTX *pCtx; 
    RTMP_SESSION *pServer;
    
    if(iEvent|EPOLLIN)
    {
        iNewFd = accept(iFd, &tmpAddr, (socklen_t *)&iSocketSize); 
        if(RTMP_EPOLLSRV_INVALIDFD < iNewFd)    
        {
            pServer = (RTMP_SESSION *)malloc(sizeof(RTMP_SESSION));
            if(NULL == pServer)
            {
                return -1;
            }
            pServer->handshake  = 0; 
            pCtx = (EPOLL_CTX *)malloc(sizeof(EPOLL_CTX));
            if(NULL == pCtx)
            {
                free(pServer);
                return -1;
            }
            pServer->socket = iNewFd;
            pCtx->iFd = iNewFd;
            pCtx->pContext = pServer;
            pCtx->pfHandle = RtmpSessionHandle;
            /* 加入epoll */
            iRet = epoll_op(g_iEpollFd, EPOLL_CTL_ADD, iNewFd,  EPOLLIN|EPOLLERR|EPOLLHUP,  pCtx);
            
        }
        else
        {
            printf("accept errno:%s",strerror(errno));
        }
    }

    return iRet;
}


int epoll_op(int iEpollFd, int iOp, int iFd, int iEvent,  EPOLL_CTX *pCtx)
{
    int iRet;
    struct epoll_event ev;

    ev.events = iEvent;
    ev.data.ptr = pCtx;
    
    iRet = epoll_ctl(iEpollFd, iOp, iFd, &ev);
    
    return iRet;
}


int epoll_loop(int iEpollFd)
{
    int iNum;
    struct epoll_event astEpEvent[RTMP_EPOLLSRV_MAXEPOLL];
    int i;
    EpollCallBack_PF pfHandle;
    EPOLL_CTX *pCtx;
    for( ;  ;)
    {
        iNum= epoll_wait(iEpollFd, &astEpEvent[0],  RTMP_EPOLLSRV_MAXEPOLL, -1);
        if( 0 < iNum)
        {
            for(i = 0; i < iNum; i++)
            {
                pCtx = (EPOLL_CTX *)astEpEvent[i].data.ptr;
                pfHandle = pCtx->pfHandle;
                (void)pfHandle(pCtx->iFd, astEpEvent[i].events, pCtx->pContext);
            }
        }
        else
        {
            printf("epoll_wait failed\r\n");
        }
    }

    return 0;
}

int main(void)
{
    int iFd;
    struct sockaddr_in addr;
    
    printf("in the main\r\n");
    /* 初始化epoll */
    g_iEpollFd = epoll_create(200);
    if(RTMP_EPOLLSRV_INVALIDFD >= g_iEpollFd)
    {
        printf("create epoll failed\r\n");
        return  -1;
    }

    
    /* 创建侦听端口 */
    iFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(RTMP_EPOLLSRV_INVALIDFD >= iFd)
    {
        printf("create listen socket failed\r\n");
        return  -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(g_cRtmpSrvAddr);
    addr.sin_port = htons(g_usRtmpSrvPort);

    if( 0 != bind(iFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)))
    {
        return  -1;  
    }


    if( 0 != listen(iFd, 200))
    {
        return  -1;
    }

    EPOLL_CTX *pEpollCtx = (EPOLL_CTX *)malloc(sizeof(EPOLL_CTX));
    if(NULL == pEpollCtx)
    {
        return -1;
    }

    pEpollCtx->iFd = iFd;
    pEpollCtx->pfHandle = ListenHandle;
    pEpollCtx->pContext = NULL;
    /* 加入epoll */
    if(0 != epoll_op(g_iEpollFd, EPOLL_CTL_ADD, iFd, EPOLLIN|EPOLLERR|EPOLLHUP, pEpollCtx))
    {
        return -1;
    }

    g_iListenFd = iFd;

    epoll_loop(g_iEpollFd);
    

    
    return  0;
    
}
{%endhighlight c %}


### 功能 

上述代码代码主要通过epoll实现一个最基本的网络服务器（侦听一个端口，处理这个端口上连接）

几个重要数据结构

### 实现分析  

简单说明如下：

用下面的结构体EPOLL_CTX保存epoll的回调及异步处理的上下文 
```
typedef struct 
{
    int iFd; 
    EpollCallBack_PF pfHandle;
    void  *pContext;
}EPOLL_CTX;
```

从面向过程编程角度简单梳理一下epoll相关的代码
1. 创建epoll
2. 加入epoll
3. 进入epoll_loop,处理epoll事件 



## go 编程示例
 
{% highlight Go linenos %} 
package main

import (
    "fmt"
    "net"
    "os"
)

const (
    CONN_HOST = "localhost"
    CONN_PORT = "3333"
    CONN_TYPE = "tcp"
)

func main() {
    // Listen for incoming connections.
    l, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
    if err != nil {
        fmt.Println("Error listening:", err.Error())
        os.Exit(1)
    }
    // Close the listener when the application closes.
    defer l.Close()
    fmt.Println("Listening on " + CONN_HOST + ":" + CONN_PORT)
    for {
        // Listen for an incoming connection.
        conn, err := l.Accept()
        if err != nil {
            fmt.Println("Error accepting: ", err.Error())
            os.Exit(1)
        }
        // Handle connections in a new goroutine.
        go handleRequest(conn)
    }
}

// Handles incoming requests.
func handleRequest(conn net.Conn) {
    // Make a buffer to hold incoming data.
    buf := make([]byte, 1024)
    // Read the incoming connection into the buffer.
    reqLen, err := conn.Read(buf)
    if err != nil {
        fmt.Println("Error reading:", err.Error())
        conn.Close()
        return
    }
    fmt.Printf("recv:%s, len=%d\n", string(buf), reqLen)
    // Send a response back to person contacting us.
    conn.Write([]byte("Message received."))
    // Close the connection when you're done with it.
    conn.Close()
}
{%endhighlight Go %}


## 对比 

1. handleRequest代码对应c语言版本epoll回调，但是这个代码与业务逻辑很搭，读取报文，进行处理，返回结果这些操作可以同一个函数内（也就是同一个逻辑上面）实现，没有异步回调就是爽啊
2. go语言上编程上不需要看到epoll，也就没添加/删除epoll的操作
3. 编程模型方面无论是原生的回调还是reactor模型，go语言更符合业务的逻辑，而不需要考虑epoll相关处理
4. 够简洁明了，有着与c语言相当的性能，程序员们让我们一起go吧！

## Go Net实现分析 

通过以上对比，显而易见，go语言保证效率的情况，在易用性大大超过了c，那golang是如何实现的？下面具体分析golang的net库实现 

### goroutine调度时机 

一般情况有以下四种情况进行调度：
1. channel收发
2. 显示调用go函数 
3. 阻塞的系统调用，如read,write
4. GC 

5. network input
sleeping
channel operations or
blocking on primitives in the sync package.



### epoll使用 

先简单看一下各个epoll操作代码实现，先找到他们，再分析如何利用这些操作来完成简洁的网络编程

#### epoll初始化 

对应c语言版本的epoll_create，go语言版本在初始化在下面的代码中完成:

```
func netpollinit() {
    epfd = epollcreate1(_EPOLL_CLOEXEC)
    if epfd >= 0 {
        return
    }
    epfd = epollcreate(1024)
    if epfd >= 0 {
        closeonexec(epfd)
        return
    }
    println("netpollinit: failed to create epoll descriptor", -epfd)
    throw("netpollinit: failed to create descriptor")
}
```


#### 将fd加入epoll 
 
对应c语言版本的epoll_op，go语言版本在初始化在下面的代码中完成:
```
func netpollopen(fd uintptr, pd *pollDesc) int32 {
    var ev epollevent
    ev.events = _EPOLLIN | _EPOLLOUT | _EPOLLRDHUP | _EPOLLET
    *(**pollDesc)(unsafe.Pointer(&ev.data)) = pd
    return -epollctl(epfd, _EPOLL_CTL_ADD, int32(fd), &ev)
}
```

注意这里采用的边沿触发


#### 从epoll摘除fd  

对应c语言版本的epoll_op，go语言版本在初始化在下面的代码中完成:

```
func netpollclose(fd uintptr) int32 {
    var ev epollevent
    return -epollctl(epfd, _EPOLL_CTL_DEL, int32(fd), &ev)
}
```



#### 数据结构 pollDesc

结构体pollDesc用于关联fd与epoll，具体结构如下：

```
// Network poller descriptor.
// 每个添加到epoll中的fd都对应了一个PollDesc结构实例
type pollDesc struct {
    // 指向下一个pollDesc
    link *pollDesc // in pollcache, protected by pollcache.lock

    // The lock protects pollOpen, pollSetDeadline, pollUnblock and deadlineimpl operations.
    // This fully covers seq, rt and wt variables. fd is constant throughout the PollDesc lifetime.
    // pollReset, pollWait, pollWaitCanceled and runtime·netpollready (IO readiness notification)
    // proceed w/o taking the lock. So closing, rg, rd, wg and wd are manipulated
    // in a lock-free way by all operations.
    // NOTE(dvyukov): the following code uses uintptr to store *g (rg/wg),
    // that will blow up when GC starts moving objects.
    lock mutex // protects the following fields
    // 系统为socket分配的fd
    fd      uintptr
    closing bool // 是否关闭
    // 用于保护旧定时器和就绪的通知
    seq uintptr // protects from stale timers and ready notifications
    // 网络io读状态，分为三种: 网络io就绪， 进入等待状态， 等待状态，此时rg保存等待goroutine实例的指针
    rg uintptr // pdReady, pdWait, G waiting for read or nil
    rt timer   // read deadline timer (set if rt.f != nil)
    // 读超时时间，单位为ns
    rd int64 // read deadline
    // 网络io写状态，分为三种: 网络io就绪， 进入等待状态， 等待状态，此时rg保存等待goroutine实例的指针
    wg uintptr // pdReady, pdWait, G waiting for write or nil
    // 写超时时间，单位为ns
    wt   timer  // write deadline timer
    wd   int64  // write deadline
    user uint32 // user settable cookie
}
```



#### epoll操作封装  

以初始化epoll及加入epoll为例，在下面的函数中完成的

```

var serverInit sync.Once

func (pd *pollDesc) init(fd *netFD) error {
    serverInit.Do(runtime_pollServerInit)
    ctx, errno := runtime_pollOpen(uintptr(fd.sysfd))
    if errno != 0 {
        return syscall.Errno(errno)
    }
    pd.runtimeCtx = ctx
    return nil
}
```


```
func (fd *netFD) accept() (netfd *netFD, err error) {
    if err := fd.readLock(); err != nil {
        return nil, err
    }
    defer fd.readUnlock()

    var s int
    var rsa syscall.Sockaddr
    if err = fd.pd.prepareRead(); err != nil {
        return nil, err
    }
    for {
        s, rsa, err = accept(fd.sysfd)
        if err != nil {
            nerr, ok := err.(*os.SyscallError)
            if !ok {
                return nil, err
            }
            switch nerr.Err {
            case syscall.EAGAIN:
                if err = fd.pd.waitRead(); err == nil {
                    continue
                }
            case syscall.ECONNABORTED:
                // This means that a socket on the
                // listen queue was closed before we
                // Accept()ed it; it's a silly error,
                // so try again.
                continue
            }
            return nil, err
        }
        break
    }

    if netfd, err = newFD(s, fd.family, fd.sotype, fd.net); err != nil {
        closeFunc(s)
        return nil, err
    }
    // 调用上面函数加入epoll
    if err = netfd.init(); err != nil {
        fd.Close()
        return nil, err
    }
    lsa, _ := syscall.Getsockname(netfd.sysfd)
    netfd.setAddr(netfd.addrFunc()(lsa), netfd.addrFunc()(rsa))
    return netfd, nil
}
```


#### 读处理

先从为读入口代码开始
```

func (fd *netFD) Read(p []byte) (n int, err error) {
    if err := fd.readLock(); err != nil {
        return 0, err
    }
    defer fd.readUnlock()
    if len(p) == 0 {
        // If the caller wanted a zero byte read, return immediately
        // without trying. (But after acquiring the readLock.) Otherwise
        // syscall.Read returns 0, nil and eofError turns that into
        // io.EOF.
        // TODO(bradfitz): make it wait for readability? (Issue 15735)
        return 0, nil
    }
    if err := fd.pd.prepareRead(); err != nil {
        return 0, err
    }
    for {
        n, err = syscall.Read(fd.sysfd, p)
        if err != nil {
            n = 0
            if err == syscall.EAGAIN { // 没有可读数据，进行读等待处理
                if err = fd.pd.waitRead(); err == nil {
                    continue
                }
            }
        }
        err = fd.eofError(n, err)
        break
    }
    if _, ok := err.(syscall.Errno); ok {
        err = os.NewSyscallError("read", err)
    }
    return
}

```


具体分析读等待处理,跳过封装，直接分析处理代码：

```
//go:linkname net_runtime_pollWait net.runtime_pollWait
// 进入pollwait状态进行goroutine调度
func net_runtime_pollWait(pd *pollDesc, mode int) int {
    err := netpollcheckerr(pd, int32(mode))
    if err != 0 {
        return err
    }
    // As for now only Solaris uses level-triggered IO.
    if GOOS == "solaris" {
        netpollarm(pd, mode)
    }
    // 
    for !netpollblock(pd, int32(mode), false) {
        err = netpollcheckerr(pd, int32(mode))
        if err != 0 {
            return err
        }
        // Can happen if timeout has fired and unblocked us,
        // but before we had a chance to run, timeout has been reset.
        // Pretend it has not happened and retry.
    }
    return 0
}

```


```
// returns true if IO is ready, or false if timedout or closed
// waitio - wait only for completed IO, ignore errors
// 返回true表示IO就绪，返回false表示超时或者关闭
// waitio 表示是否等待IO,超时时该参数为false
func netpollblock(pd *pollDesc, mode int32, waitio bool) bool {
    // 从pd.rg取指针
    gpp := &pd.rg
    if mode == 'w' {
        gpp = &pd.wg
    }

    // set the gpp semaphore to WAIT
    for {
        old := *gpp
        if old == pdReady {
            *gpp = 0
            return true
        }
        if old != 0 {
            throw("netpollblock: double wait")
        }
        // cas 设置读状态为pdWait状态
        if atomic.Casuintptr(gpp, 0, pdWait) {
            break
        }
    }

    // need to recheck error states after setting gpp to WAIT
    // this is necessary because runtime_pollUnblock/runtime_pollSetDeadline/deadlineimpl
    // do the opposite: store to closing/rd/wd, membarrier, load of rg/wg
    // 因为runtime_pollUnblock runtime_pollSetDeadline/deadlineimpl 将rg/wg状态修改为closing
    if waitio || netpollcheckerr(pd, mode) == 0 {
        gopark(netpollblockcommit, unsafe.Pointer(gpp), "IO wait", traceEvGoBlockNet, 5)
    }
    // be careful to not lose concurrent READY notification
    old := atomic.Xchguintptr(gpp, 0)
    if old > pdWait {
        throw("netpollblock: corrupted state")
    }
    return old == pdReady
}
```

 


### 总结  

1. net网络库的设计的精华,良好的封装与接口,提高简单可靠的接口
2. 充分利用goroutine机制 
3. 同步编程，异步执行，这一点其实在内核也能找到，只是调度机制不一样 
4. 多学习源码，这里面有精妙的设计，科学的框架
5. 很多问题深入一下就到底层了 
6. 异步编程能够带来高性能，但是也是高要求，如果系统复杂，出现问题不好定位，同时代码的可读性也差 
7. 代码在满足正确性的基础上，应先追求可读性，规范性，高性能往后排 



### 参考 

1. [How Goroutines Work](http://blog.nindalf.com/how-goroutines-work/) 
