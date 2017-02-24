---
layout: post
title:  TCP连接下载文件失败，也许该看看这篇文章
categories:  TCP/IP
tags:  TCP/IP  
---  

体现一下标题党，现在自媒体横行，容我也放肆一回（多了我也不行，替自己码字能力捉急）！

另起一行，到此为止，进入正题

### 缘起  

最近通过uc浏览器下载apk的时候，偶尔出现下载apk，下载了60%左右卡住，心想还有这种问题,[The curious case of slow downloads](https://blog.cloudflare.com/the-curious-case-of-slow-downloads/)这篇文章一定没有引起广大程序员们注意，特别是研发CDN同学的注意。（PS：毕竟这个问题不是常出现，就算一次下载失败，反正可以重新下载，总能下载成功的）


### 说明 

由于本人英文水平有限，翻译水平更是不足，就不具体翻译上面的文章，仅作简单说明，更深的理解请阅读[The curious case of slow downloads](https://blog.cloudflare.com/the-curious-case-of-slow-downloads/) 


### 问题描述

Cloudflare是美国一家CDN厂商，他们的工程师发现下面的问题：
>一些下载速度很慢的连接被突然关闭，导致用户下载失败
>这些连接不是客户端主动关闭，而是服务端主动关闭的


### 问题原因 

原文有具体解决这个问题详细过程，还是值得一看，这里不作描述。

在满足如下条件情况下会出现下载失败：

1. socket发送缓冲区可用空间低于缓冲区总大小的三分之一 
2. 用户下载速度不能达到在60秒内使该socket发送缓冲区可用空间超过缓冲区总大小的三分之一  
3. nginx配置项send_timeout对应值为60秒

当满足上述条件，当60秒超时后，nginx会关闭该连接

这里大家可能有一个问题，网速慢也会不断发送，怎么会超时出现关闭连接？

这里有一个普遍的误解：认为send像recv那样，每发送成功一个都上报epoll事件（以linux为例）,而实际send上报epoll事件条件如下：

1. send buffer有可用发送空间   
2. 进入发送队列的数据一定要低于LOWAT的设置值（注意：linux 内核2.6版本没有这个限制，linux 内核4.5版本以上有此条件，其他版本情况未知） 
3. 发送缓存区的可用空间一定要超过大于发送空间的已使用的空间的二分之一 

其中第三个条件对应内核代码如下：

```
unsigned int tcp_poll(struct file *file, struct socket *sock, poll_table *wait)
{
    unsigned int mask;
    struct sock *sk = sock->sk;
    struct tcp_sock *tp = tcp_sk(sk);

    sock_poll_wait(file, sk->sk_sleep, wait);
    if (sk->sk_state == TCP_LISTEN) // 侦听状态，进入listen poll，即检查侦听socket的accpet队列是否为空
        return inet_csk_listen_poll(sk);

    /* Socket is not locked. We are protected from async events
     * by poll logic and correct handling of state changes
     * made by other threads is impossible in any case.
     */

    mask = 0;

    /*
     * POLLHUP is certainly not done right. But poll() doesn't
     * have a notion of HUP in just one direction, and for a
     * socket the read side is more interesting.
     *
     * Some poll() documentation says that POLLHUP is incompatible
     * with the POLLOUT/POLLWR flags, so somebody should check this
     * all. But careful, it tends to be safer to return too many
     * bits than too few, and you can easily break real applications
     * if you don't tell them that something has hung up!
     *
     * Check-me.
     *
     * Check number 1. POLLHUP is _UNMASKABLE_ event (see UNIX98 and
     * our fs/select.c). It means that after we received EOF,
     * poll always returns immediately, making impossible poll() on write()
     * in state CLOSE_WAIT. One solution is evident --- to set POLLHUP
     * if and only if shutdown has been made in both directions.
     * Actually, it is interesting to look how Solaris and DUX
     * solve this dilemma. I would prefer, if POLLHUP were maskable,
     * then we could set it on SND_SHUTDOWN. BTW examples given
     * in Stevens' books assume exactly this behaviour, it explains
     * why POLLHUP is incompatible with POLLOUT.    --ANK
     *
     * NOTE. Check for TCP_CLOSE is added. The goal is to prevent
     * blocking on fresh not-connected or disconnected socket. --ANK
     */
     /* socket 与tcp 状态转化 poll事件 */
    if (sk->sk_shutdown == SHUTDOWN_MASK || sk->sk_state == TCP_CLOSE)
        mask |= POLLHUP;
    if (sk->sk_shutdown & RCV_SHUTDOWN)
        mask |= POLLIN | POLLRDNORM | POLLRDHUP;

    /* Connected? */
    if ((1 << sk->sk_state) & ~(TCPF_SYN_SENT | TCPF_SYN_RECV)) {
        int target = sock_rcvlowat(sk, 0, INT_MAX);

        if (tp->urg_seq == tp->copied_seq &&
            !sock_flag(sk, SOCK_URGINLINE) &&
            tp->urg_data)
            target--;

        /* Potential race condition. If read of tp below will
         * escape above sk->sk_state, we can be illegally awaken
         * in SYN_* states. */
         /*
         未处理接收报文字节数超过了最小阈值，满足可读条件
         */
        if (tp->rcv_nxt - tp->copied_seq >= target)
            mask |= POLLIN | POLLRDNORM;

        if (!(sk->sk_shutdown & SEND_SHUTDOWN)) {
            /*  
            sk->sk_sndbuf - sk->sk_wmem_queued >=   sk->sk_wmem_queued>>1 (简单理解为最大值 0.5* sk->sk_wmem_queued)
            如果未发送报文超过了66%，那么不会继续上报POLLOUT事件
            */
            if (sk_stream_wspace(sk) >= sk_stream_min_wspace(sk)) {
                mask |= POLLOUT | POLLWRNORM;
            } else { 
                /* send SIGIO later */
                /* 发送SIGIO  */
                set_bit(SOCK_ASYNC_NOSPACE,
                    &sk->sk_socket->flags);
                set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);

                /* Race breaker. If space is freed after
                 * wspace test but before the flags are set,
                 * IO signal will be lost.
                 */
                if (sk_stream_wspace(sk) >= sk_stream_min_wspace(sk))
                    mask |= POLLOUT | POLLWRNORM;
            }
        } else
            mask |= POLLOUT | POLLWRNORM;

        if (tp->urg_data & TCP_URG_VALID)
            mask |= POLLPRI;
    }
    /* This barrier is coupled with smp_wmb() in tcp_reset() */
    smp_rmb();
    if (sk->sk_err)
        mask |= POLLERR;

    return mask;
}
```





### 解决方案 

知道上面的原因对应可以选择方案如下：

方案一：增加send_timeout时间，例如将时间调整为280秒，可以保证在5M发送缓冲区条件下，用户下载速度超过50Kbps不会出现超时导致连接被关闭 


方案二：通过设置/proc/sys/net/ipv4/tcp_wmem值减小socket发送缓冲区大小，发送缓冲区减小，那么在一定下载速率下在指定的时间需要完成下载的大小变小，就可以避免上述的问题出现的条件  

显而易见，这种两种方案都不能从根本上解决问题，只是降低问题出现的概率（这也是进步） 

方案三：改变超时处理，而不是直接关闭，可利用ioctl(TIOCOUTQ)来获取有多少数据仍停留在发送缓冲区，调整超时时间，具体实现可以参考[The curious case of slow downloads](https://blog.cloudflare.com/the-curious-case-of-slow-downloads/)中提到的方案：[a Linux specific patch to NGINX](https://github.com/cloudflare/cloudflare-blog/blob/master/2016-03-slow-downloads/nginx_send_minimum_rate.patch)

相比较于方案一和方案二，方案三对网络速率变化与波动适应性强



### 总结  

1.  [The curious case of slow downloads](https://blog.cloudflare.com/the-curious-case-of-slow-downloads/)值得一看（感谢cloudflare工程师没有放弃一些偶现的问题）
2. nginx现有实现单一固化的处理导致不适应tcp传输的多变性
3. 在复杂多变的网络环境下，保证传输高可靠性里面需要很多技术细节需要挖掘 
4. 网络是复杂的，主要由于以下原因：

- 网络协议复杂，例如tcp 
- 网络本身不可靠 
- 网络连接多样性
- 网络要求高：低延迟，少丢包，抖动小，高速率，自适应   
 




### 参考

1. [The curious case of slow downloads](https://blog.cloudflare.com/the-curious-case-of-slow-downloads/) 
2. [linux-2.26.32](https://github.com/myself659/linux-2.26.32)    



 








