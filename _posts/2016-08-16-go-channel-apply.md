---
layout: post
title: Go channel 应用篇
categories:  Go
tags:  Go， channel
---

## Channel 
golang编程语言中用于goroutine之间同步通信一种机制


## 应用思想 

1. CSP 
2. Don't communicate by sharing memory, share memory by communicating
3. Goroutine作为执行实体，进行设计分解


##应用模型 

### 信号同步

这种情况下只关心读写操作，不关心读写的数据内容

#### 时序同步 

#### 开始通知 

```
package main

import (
    "fmt"
    "time"
)

var start = make(chan bool)

func waitforstart(i int) {
    <-start
    fmt.Println(i, ": got signal  to start")
}

func main() {
    for i := 0; i < 3; i++ {
        go waitforstart(i)
    }

    close(start)
    <-time.After(time.Second)
}

```


### 数据通信

channel作为goroutine之间一种数据通信方式，可以向channel读写任意类型的数据：

1. bool,int, int64...
2. pointer... 
3. struct...  
4. chan...  
5. interface ... 
6. map,slice,list...

#### 串行化

以序号生成器为例，参考代码如下：
```
package main

import (
    "fmt"
    "time"
)

func main() {
    ch := make(chan uint64)

    go func(ch chan uint64) {
        var seq uint64 = 0
        for {
            ch <- seq
            fmt.Println("send seq:", seq)
            seq++
        }
    }(ch)

    fmt.Println("recv seq:", <-ch)
    fmt.Println("recv seq:", <-ch)
    <-time.After(time.Second)
}
```




#### req-rsp 

#### pipeline

```
package main

import "fmt"

func counter(naturals chan<- int) {
    for x := 0; x < 5; x++ {
        naturals <- x
    }
    close(naturals)
}

func squarer(squares chan<- int, naturals <-chan int) {
    for v := range naturals {
        squares <- v * v
    }
    close(squares)
}

func printer(squares <-chan int) {
    for v := range squares {
        fmt.Println(v)
    }
}

func main() {
    naturals := make(chan int)
    squares := make(chan int)

    go counter(naturals)
    go squarer(squares, naturals)
    printer(squares)
}
```

#### Fan-In 

#### Fan-Out

#### Pub-Sub  



具体参考代码：[ChanBroker](https://github.com/myself659/ChanBroker)

channel由谁创建，对谁可见？ 
channel由谁关闭，是否需要关闭 

channel之间关系？ 

创建得到通道，第一种利用通道发送，第二种利用通道接收，第三种将通道作为参数进行通道传输
 
Channel 归属哪些goroutine   只用于两个 禁上gorouine复用  

用结构体封装channel与goroutine  这种编程方式比较好 

结构体对业务 针对业务管理goroutine 






## 后记

上面所写只是个人不吐不快的消遣之作，实际应用过程中解决问题远比上面的模型复杂，需要进行合理分解，以便方便利用上面一些模型（感觉像是套设计模式，套就套吧用简单稳定的模型解决问题才是王道），下面这个参考才是经典，对了，需要翻墙，请自备梯子

## 参考 

1. [Go Concurrency Patterns: Pipelines and cancellation](http://blog.golang.org/pipelines)
2. [Advanced Go Concurrency Patterns](https://talks.golang.org/2013/advconc.slide)([视频](https://www.youtube.com/watch?v=QDDwwePbDtw)) 
3. [Go Concurrency Patterns](https://talks.golang.org/2012/concurrency.slide#1)([视频](https://www.youtube.com/watch?v=f6kdp27TYZs)) 