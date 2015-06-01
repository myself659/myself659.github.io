---
layout: post
title: "Linux may sleep function"
categories:
- 
tags:
- 


---
## Linux内核会引导睡眠的函数总结 ##

在linux内核开发中断处理函数不能调用可能引导睡眠的函数，下面总结linux内核可能引起睡眠的函数如下：

### schedule函数 ###
>
1. schedule\_timeout
2. schedule\_timeout\_uninterruptible
3. schedule\_timeout\_interruptible
4. cond\_resched
5. might\_resched
>
### sleep函数 ###
>
1. msleep
2. msleep_interruputible
3. ssleep
4. osal_usleep
5. might_sleep
>
### 取信号量函数 ###
>
1. down
2. down_timeout
3. down_read
4. down_write
5. down_interruptible
6. wait\_for\_completion
7. wait\_for\_completion\_interruptible
8. wait\_for\_completion\_timeout
9. wait\_for\_completion\_interruptible\_timeout
>
### kmallo相关函数含有标志GFP_KERNEL ###
>
1. kmalloc
2. kzalloc
3. krealloc
4. kmem\_cache\_create
5. kmem\_cache\_alloc
6. kmem\_cache\_zalloc
>

### 取睡眠锁函数 ###
>
1. mutex_lock
2. mutex\_lock\_timeout
3. mutex\_lock\_nested
4. mutex\_lock\_interruptible
5. mutex\_lock\_interruptible\_nested
>

### 在中断处理函数不能使用睡眠函数原因 ###
主要原因如下：

1. 中断是一种紧急事务，中断处理函数要求快
1. linux是以进程为调度单位的，调度器只看到进程内核栈，而看不到中断栈，在独立中断栈的模式下，如果linux内核在中断处理函数内发生了调度或者睡眠，导致无法找到回家的路，未执行的中断处理代码再也无法获得执行机会，中断处理未完成，中断处理未完成在不同的情景会导致不同的问题




