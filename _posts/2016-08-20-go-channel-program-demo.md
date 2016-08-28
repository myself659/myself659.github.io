---
layout: post
title: Go channel 编程篇
categories:  Go
tags:  Go， channel
---


### 简介 

ChanBroker设计主要参考Kafka模型，主要提供进程内goroutine之间通信，实现以下功能：

1. 支持多个Publisher发布内容
2. 支持Subscriber注册与去注册订阅
3. 发布内容可以是任何形式
4. ChanBroker根据订阅情况完成内容推送



### 版本1

具体代码如下:

{% highlight c linenos %}
package ChanBroker

type Content interface{}

type Subscriber chan Content

type ChanBroker struct {
    RegSub      chan Subscriber
    UnRegSub    chan Subscriber
    Contents    chan Content
    Stop        chan bool
    Subscribers map[Subscriber]bool
}

func NewChanBroker() *ChanBroker {
    ChanBroker := new(ChanBroker)
    ChanBroker.RegSub = make(chan Subscriber)
    ChanBroker.UnRegSub = make(chan Subscriber)
    ChanBroker.Contents = make(chan Content)
    ChanBroker.Stop = make(chan bool)
    ChanBroker.Subscribers = make(map[Subscriber]bool)
    ChanBroker.run()

    return ChanBroker
}

func (self *ChanBroker) run() {

    go func() { // Broker goroutine  
        for {
            select {
            case content := <-self.Contents:
                for sub := range self.Subscribers {
                    sub <- content
                }
            case sub := <-self.RegSub:
                self.Subscribers[sub] = true
            case sub := <-self.UnRegSub:
                delete(self.Subscribers, sub)
                close(sub)
            case <-self.Stop:
                for sub := range self.Subscribers {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                return  
            }
        }
    }()
}

func (self *ChanBroker) RegSubscriber() Subscriber {
    sub := make(Subscriber)
    self.RegSub <- sub
    return sub
}

func (self *ChanBroker) UnRegSubscriber(sub Subscriber) {
    self.UnRegSub <- sub
}

func (self *ChanBroker) StopPublish() {
    self.Stop <- true
}

func (self *ChanBroker) PubContent(c Content) {
    self.Contents <- c
}
{%endhighlight c %}

存在以下问题，具体如下： 

#### 问题1：不支持扩展 

**问题描述**：
在一个Broker goroutine内完成注册与去注册以及内容发布推送给Subscriber，无法控制Subscriber数量，且不支持扩展

**解决思路**：
主要修改如下：

1. 增加Pusher goroutine，Pusher goroutine支持动态创建，由Pusher goroutine完成具体内容的推送


#### 问题2： 推送内容到Subscriber存在Deadlock风险

**问题描述**：
例如一个Subscriber不能正确从通道接收订阅内容，那么Broker会阻塞在上述代码的34行，与此同时其他Subscriber都会阻塞，极有可能引起级联阻塞，影响恶劣

**解决思路**：

主要修改如下：

1. 支持Subscriber可以定制订阅通道的大小，利用队列缓存内容
2. 增加Pusher goroutine，避免由于某个内容的推送导致整个内容推送的阻塞
3. 增加超时机制，避免在具体内容推送过程由于某一个Subscriber不正常工作影响其他的Subscriber

#### 问题3： Broker goroutine退出后调用RegSubscriber，UnRegSubscriber，StopPublish，PubContent函数存在Deadlock风险

**问题描述**：

如题 


**解决思路**：

主要修改如下：

1. ChanBroker增加退出状态描述，避免Broker goroutine退出之后上述函数向Broker goroutine 的channel发送信息 


### 版本2

具体代码如下:

{% highlight c linenos %}
package ChanBroker

import (
    "fmt"
    "sync"
    "time"
)

type Content interface{}

type Subscriber chan Content

type ChanBroker struct {
    RegSub      chan Subscriber
    UnRegSub    chan Subscriber
    Contents    chan Content
    Stop        chan bool
    exit        bool
    Subscribers map[Subscriber]bool
    lock        sync.RWMutex
    timeout     time.Duration
}

func NewChanBroker(timeout time.Duration) *ChanBroker {
    ChanBroker := new(ChanBroker)
    ChanBroker.RegSub = make(chan Subscriber)
    ChanBroker.UnRegSub = make(chan Subscriber)
    ChanBroker.Contents = make(chan Content)
    ChanBroker.Stop = make(chan bool)
    ChanBroker.exit = false
    ChanBroker.Subscribers = make(map[Subscriber]bool)
    ChanBroker.timeout = timeout
    ChanBroker.run()

    return ChanBroker
}

func (self *ChanBroker) run() {

    go func() { // Broker goroutine 
        for {
            select {
            case content := <-self.Contents:
                go func() { // Pusher goroutine 
                    self.lock.RLock()
                    for sub := range self.Subscribers {
                        select {
                        case sub <- content:
                        case <-time.After(self.timeout):
                            fmt.Println(sub, "time out ")
                        }

                    }
                    self.lock.RUnlock()
                }()
            case sub := <-self.RegSub:
                self.lock.Lock()
                self.Subscribers[sub] = true
                self.lock.Unlock()
            case sub := <-self.UnRegSub:
                self.lock.Lock()
                delete(self.Subscribers, sub)
                self.lock.Unlock()
                close(sub) // may be close of closed channel
            case <-self.Stop:
                if self.exit == false {
                    self.exit = true
                    close(self.Stop)
                    self.lock.Lock()
                    for sub := range self.Subscribers {
                        delete(self.Subscribers, sub) // 必须先删除再close
                        close(sub)
                    }
                    self.lock.Unlock()

                    return // exit goroutine
                }
            }
        }
    }()
}

func (self *ChanBroker) RegSubscriber(size uint) Subscriber {
    if self.exit == true {
        return nil
    }
    sub := make(Subscriber, size)
    self.RegSub <- sub // maybe block
    return sub
}

func (self *ChanBroker) UnRegSubscriber(sub Subscriber) {
    if self.exit == true {
        return
    }
    self.UnRegSub <- sub // maybe block
}

func (self *ChanBroker) StopPublish() {
    if self.exit == true {
        return
    }
    self.Stop <- true // maybe panic
}

func (self *ChanBroker) PubContent(c Content) {
    self.Contents <- c // maybe block
}

{%endhighlight c %}

存在以下问题，具体如下：

#### 问题1：存在 panic:close of closed channel的风险

**问题描述**：

如果Subscriber goroutine两次调用UnRegSubscriber，就会发生close of closed channel，导致panic 

相应代码如下：

{% highlight c linenos %}
  case sub := <-self.UnRegSub:
                self.lock.Lock()
                delete(self.Subscribers, sub)
                self.lock.Unlock()
                close(sub) // may be close of closed channel
{%endhighlight c %}



**解决思路**：

很简单，关闭前检查Subscriber对应channel是否订阅map当中，代码如下：

{% highlight c linenos %}
case sub := <-self.UnRegSub:
                self.lock.Lock()
                _, ok := self.Subscribers[sub]
                if ok {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                self.lock.Unlock()
{%endhighlight c %}

#### 问题2：不靠谱的exit标记

**问题描述**：
exit标记不能同步goroutines对Stop通道的写操作与关闭操作，具体分析如下：

写操作代码如下：

{% highlight c linenos %}
func (self *ChanBroker) RegSubscriber(size uint) Subscriber {
    if self.exit == true {
        return nil
    }
    sub := make(Subscriber, size)
    self.RegSub <- sub // maybe block
    return sub
}
{%endhighlight c %} 

关闭操作代码段如下： 

{% highlight c linenos %}
 case <-self.Stop:
                if self.exit == false {
                    self.exit = true
                    close(self.Stop)
                    self.lock.Lock()
                    for sub := range self.Subscribers {
                        delete(self.Subscribers, sub) // 必须先删除再close
                        close(sub)
                    }
                    self.lock.Unlock()

                    return // exit goroutine
                }
{%endhighlight c %} 

在多核多个goroutine并发情况，极小概率会发生如下情况：

1.  某同一个时刻，CPU0 运行RegSubscriber goroutine检查exit标记为false，CPU0继续运行，CPU1运行到关闭操作代码段第1行，继续运行
2.  CPU0 继续运行RegSubscriber goroutine，直到阻塞在第6行, 切换运行其他goroutine，CPU 1 关闭Stop Channel,并退出broken goroutine
3.  CPU0 继续运行其他goroutine，RegSubscriber goroutine一直阻塞在第6行,等待 broken goroutine 读Stop channel
4.  CPU0 继续执行其他goroutine，RegSubscriber goroutine一直阻塞在第6行，,等待 broken goroutine 读Stop channel
5.  CPU0 继续执行其他goroutine，RegSubscriber goroutine一直阻塞在第6行，,等待 broken goroutine 读Stop channel
6.  ... 



**解决思路**：

不用exit标记了，将状态关联到Stop Channel状态上（这又掉到另一个坑里）


#### 问题3：不能保证Subscribers有序的接收消息

**问题描述**：  
版本2中每接收一个Content都会启动一个Push goroutine,这些Push goroutine执行是无序执行的，有序的内容推送需求遇上了无序的goroutine，自然有问题了

**解决思路**： 

方案1：无序推送，由Content自身加上序号，同时由各个Subscriber处理逻辑根据序号保证有序处理Content

方案2：每一个Subcriber有一个Content队列，保证有序推送，简化各个Subscriber处理逻辑，具体实现见版本5


### 版本3  

{% highlight c linenos %}
package ChanBroker

import (
    "errors"
    "fmt"
    "sync"
    "time"
)

type Content interface{}

type Subscriber chan Content

type ChanBroker struct {
    RegSub      chan Subscriber
    UnRegSub    chan Subscriber
    Contents    chan Content
    Stop        chan bool
    exit        bool
    Subscribers map[Subscriber]bool
    lock        sync.RWMutex
    timeout     time.Duration
}

var errBrokerExit error = errors.New("Broker exit")
var errTimeOut error = errors.New("Time out")

func NewChanBroker(timeout time.Duration) *ChanBroker {
    ChanBroker := new(ChanBroker)
    ChanBroker.RegSub = make(chan Subscriber)
    ChanBroker.UnRegSub = make(chan Subscriber)
    ChanBroker.Contents = make(chan Content)
    ChanBroker.Stop = make(chan bool)
    ChanBroker.exit = false
    ChanBroker.Subscribers = make(map[Subscriber]bool)
    ChanBroker.timeout = timeout
    ChanBroker.run()

    return ChanBroker
}

func (self *ChanBroker) run() {

    go func() {
        for {
            select {
            case content := <-self.Contents:
                go func() {
                    self.lock.RLock()
                    for sub := range self.Subscribers {
                        select {
                        case sub <- content:
                        case <-time.After(self.timeout):
                            fmt.Println(sub, "time out ")
                        }

                    }
                    self.lock.RUnlock()
                }()

            case sub := <-self.RegSub:
                self.lock.Lock()
                self.Subscribers[sub] = true
                self.lock.Unlock()

            case sub := <-self.UnRegSub:
                self.lock.Lock()
                _, ok := self.Subscribers[sub]
                if ok {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                self.lock.Unlock()

            case <-self.Stop:
                close(self.Stop)
                self.lock.Lock()
                for sub := range self.Subscribers {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                self.lock.Unlock()

                return // exit goroutine
            }
        }
    }()
}

func (self *ChanBroker) RegSubscriber(size uint) (Subscriber, error) {
    select {
    case _, ok := <-self.Stop:
        if ok == false {
            return nil, errBrokerExit
        } else {
            sub := make(Subscriber, size)
            self.RegSub <- sub
            return sub, nil
        }
    case <-time.After(self.timeout):
        return nil, errTimeOut
    default:
        sub := make(Subscriber, size)
        self.RegSub <- sub
        return sub, nil
    }

}

func (self *ChanBroker) UnRegSubscriber(sub Subscriber) {
    select {
    case _, ok := <-self.Stop:
        if ok == false {
            return
        } else {
            self.UnRegSub <- sub
            return
        }
    case <-time.After(self.timeout):
        return
    default:
        self.UnRegSub <- sub
        return
    }

}

func (self *ChanBroker) StopPublish() {
    select {
    case _, ok := <-self.Stop:
        if ok == false {
            return
        } else {
            self.Stop <- true
            return
        }
    default:
        self.Stop <- true 
        return
    }
}

func (self *ChanBroker) PubContent(c Content) error {
    select {
    case _, ok := <-self.Stop:
        if ok == false {
            return errBrokerExit
        } else {
            self.Contents <- c
            return nil
        }
    case <-time.After(self.timeout):
        return errTimeOut
    default:
        self.Contents <- c
        return nil
    }

}
{%endhighlight c %} 


存在以下问题，具体如下：  

#### 问题1： 读channel出现错误的竞争

**问题描述**：

版本3的一个错误的方案选择，导致Stop Channel出现读竞争，导致不能停止发布，出现goroutine leak，分析如下：

1.  Stop Channel 只会写一次  
2.  Stop Channel 却有多个读goroutine
3.  基于上面有情况，并不能保证Broker goroutine 能收到停止发布的信息，如果被其他goroutine收到，导致Broker goroutine收不到结束消息，进而不能关闭所有Subscriber，导致所有Subscriber goroutine泄露 



**解决思路**：

版本3引入关闭状态是一个错误的设计，引入状态，也就引入对依赖状态的对象，增加了代码的复杂性，状态的维护容易导致bug，在设计与代码应当追求无状态设计（好处多多）, 这里采用超时来解决，如果超时用由调用者确定处理策略



### 版本4 

{% highlight c linenos %}
package ChanBroker

import (
    "errors"
    "fmt"
    "sync"
    "time"
)

type Content interface{}

type Subscriber chan Content

type ChanBroker struct {
    RegSub      chan Subscriber
    UnRegSub    chan Subscriber
    Contents    chan Content
    Stop        chan bool
    exit        bool
    Subscribers map[Subscriber]bool
    lock        sync.RWMutex
    timeout     time.Duration
}

var errBrokerExit error = errors.New("ChanBroker exit")
var errTimeOut error = errors.New("ChanBroker Time out")

func NewChanBroker(timeout time.Duration) *ChanBroker {
    ChanBroker := new(ChanBroker)
    ChanBroker.RegSub = make(chan Subscriber)
    ChanBroker.UnRegSub = make(chan Subscriber)
    ChanBroker.Contents = make(chan Content)
    ChanBroker.Stop = make(chan bool)
    ChanBroker.exit = false
    ChanBroker.Subscribers = make(map[Subscriber]bool)
    ChanBroker.timeout = timeout
    ChanBroker.run()

    return ChanBroker
}

func (self *ChanBroker) run() {

    go func() {
        for {
            select {
            case content := <-self.Contents:
                go func() {
                    self.lock.RLock()
                    for sub := range self.Subscribers {
                        select {
                        case sub <- content:
                        case <-time.After(self.timeout):
                            fmt.Println(sub, "time out ")
                        }

                    }
                    self.lock.RUnlock()
                }()

            case sub := <-self.RegSub:
                self.lock.Lock()
                self.Subscribers[sub] = true
                self.lock.Unlock()

            case sub := <-self.UnRegSub:
                self.lock.Lock()
                _, ok := self.Subscribers[sub]
                if ok {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                self.lock.Unlock()

            case <-self.Stop:
                // close(self.Stop)
                self.lock.Lock()
                for sub := range self.Subscribers {
                    delete(self.Subscribers, sub)
                    close(sub)
                }
                self.lock.Unlock()

                return // exit goroutine
            }
        }
    }()
}

func (self *ChanBroker) RegSubscriber(size uint) (Subscriber, error) {
    sub := make(Subscriber, size)
    select {
    case <-time.After(self.timeout):
        return nil, errTimeOut
    case self.RegSub <- sub:
        return sub, nil
    }

}

func (self *ChanBroker) UnRegSubscriber(sub Subscriber) {
    select {
    case <-time.After(self.timeout):
        return
    case self.UnRegSub <- sub:
        return
    }

}

func (self *ChanBroker) StopPublish() error {
    select {
    case self.Stop <- true:
        return nil
    case <-time.After(self.timeout):
        return errTimeOut
    }
}

func (self *ChanBroker) PubContent(c Content) error {
    select {
    case <-time.After(self.timeout):
        return errTimeOut
    case self.Contents <- c:
        return nil
    }

}
{%endhighlight c %} 

存在以下问题，具体如下：  

#### 问题1：timeout问题   

**问题描述：**

1. timeout 导致长时间持有读锁 
2. timeout 导致消息丢失，并没有推送成功 
3. 上一个Subscriber超时影响后面Subscriber内容的实时性

**解决方案：**

1. 采用select实现channel非阻塞写 
2. 对于非阻塞写失败，加入Subscriber对应的链表
3. 非阻塞写保证避免Subscribers之间相互影响


#### 问题2：锁问题 

**问题描述：** 
1. 每个Pusher goroutine持有读锁，一定情况下会成为性能的瓶颈

**解决方案：**

1. 在上面避免阻塞的基础上，只有一个goroutine来推送内容 



### 版本5 

{% highlight c linenos %}
package ChanBroker

import (
    "container/list"
    "errors"
    "fmt"
    "time"
)

type Content interface{}

type Subscriber chan Content

type ChanBroker struct {
    RegSub      chan Subscriber
    UnRegSub    chan Subscriber
    Contents    chan Content
    Stop        chan bool
    Subscribers map[Subscriber]*list.List
    timeout     time.Duration
    cachenum    uint
    timerchan   chan time.Time
}

var ErrBrokerExit error = errors.New("ChanBroker exit")
var ErrPublishTimeOut error = errors.New("ChanBroker Pulish Time out")
var ErrRegTimeOut error = errors.New("ChanBroker Reg Time out")
var ErrStopTimeOut error = errors.New("ChanBroker Stop Publish Time out")

func NewChanBroker(timeout time.Duration) *ChanBroker {
    ChanBroker := new(ChanBroker)
    ChanBroker.RegSub = make(chan Subscriber)
    ChanBroker.UnRegSub = make(chan Subscriber)
    ChanBroker.Contents = make(chan Content)
    ChanBroker.Stop = make(chan bool)

    ChanBroker.Subscribers = make(map[Subscriber]*list.List)
    ChanBroker.timeout = timeout
    ChanBroker.cachenum = 0
    ChanBroker.timerchan = nil
    ChanBroker.run()

    return ChanBroker
}

func (self *ChanBroker) onContentPush(content Content) {
    for sub, clist := range self.Subscribers {

        for elem := clist.Front(); elem != nil; elem = elem.Next() {
            select {
            case sub <- elem.Value:
                if self.cachenum > 0 {
                    self.cachenum--
                }
            default:
                break // block
            }
        }

        len := clist.Len()
        if len == 0 {
            select {
            case sub <- content:
            default:
                contentList.PushBack(content)
                cachenum++
            }
        } else {
            contentList.PushBack(content)
            cachenum++
        }
    }

    if cachenum > 0 && self.timerchan == nil {
        timer := time.NewTimer(self.timeout)
        self.timerchan = timer.C
    }

}

func (self *ChanBroker) onTimerPush() {
    for sub, clist := range self.Subscribers {

        for elem := clist.Front(); elem != nil; elem = elem.Next() {
            select {
            case sub <- elem.Value:
                if self.cachenum > 0 {
                    self.cachenum--
                }
            default:
                break // block
            }
        }
    }

    if self.cachenum > 0 {
        timer := time.NewTimer(self.timeout)
        self.timerchan = timer.C
    } else {
        self.timerchan = nil
    }
}

func (self *ChanBroker) run() {

    go func() { // Broker Goroutine
        for {
            select {
            case content := <-self.Contents:
                onContentPush(content)
            case <-self.timerchan:
                onTimerPush()
            case sub := <-self.RegSub:
                clist := list.New()
                self.Subscribers[sub] = clist

            case sub := <-self.UnRegSub:
                _, ok := self.Subscribers[sub]
                if ok {
                    delete(self.Subscribers, sub)
                    close(sub)
                }

            case <-self.Stop:
                for sub := range self.Subscribers {
                    delete(self.Subscribers, sub)
                    close(sub)
                }

                return // exit goroutine
            }
        }
    }()
}

func (self *ChanBroker) RegSubscriber(size uint) (Subscriber, error) {
    sub := make(Subscriber, size)
    select {
    case <-time.After(self.timeout):
        return nil, ErrRegTimeOut
    case self.RegSub <- sub:
        return sub, nil
    }

}

func (self *ChanBroker) UnRegSubscriber(sub Subscriber) {
    select {
    case <-time.After(self.timeout):
        return
    case self.UnRegSub <- sub:
        return
    }

}

func (self *ChanBroker) StopPublish() error {
    select {
    case self.Stop <- true:
        return nil
    case <-time.After(self.timeout):
        return ErrStopTimeOut
    }
}

func (self *ChanBroker) PubContent(c Content) error {
    select {
    case <-time.After(self.timeout):
        return ErrPublishTimeOut
    case self.Contents <- c:
        return nil
    }

}
{%endhighlight c %}


存在以下问题，具体如下：

#### 问题1：不支持扩展 

**问题描述**：

在一个Broker goroutine内完成注册与去注册以及内容发布推送给Subscriber，无法控制Subscriber数量，且不支持扩展

**解决思路**：

1. 业务场景：chanbroker是进程内goroutines之间pub-sub通信方式一种实现方案，一个goroutine占用一个核来处理能满足绝太多数需求 
2. 即使不能满足极个别需求，也可以选择创建多个ChanBroker来实现扩展


### channel编程总结 

1. 避免Panic，参考[Go channel 特点篇](http://myself659.github.io/go-channel-feature/) 
2. 最大程度保证非阻塞 
3. 若非业务需要，避免channel之间读写竞争
4. channel使用很灵活，也容易出错，建议多在设计上下功夫，分解问题采用简单的模型来解决问题
5. 避免多余的channel状态引入,例如关闭channel 
6. 要有并发意识，由代码想到goroutine的运行 
7. 禁止通道复用，避免复用带来的复杂性 
8. 初期测试很重要，而且应作到充分测试 
9. Don't communicate by sharing memory, share memory by communicating  



由于个人水平有限，有什么不足与错误，敬请指正！  



