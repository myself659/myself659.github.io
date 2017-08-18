---
layout: post
title: Linux进程诊断小结
categories: Linux
tags: Linux 
---

日常工作中最常见问题是如何诊断一个进程运行过程中出现的问题，下面的总结从进程诊断的角度来展示，而是不从工具与命令角度来展示，进程诊断是工作的主体，工具与命令只是工具。

## 进程信息

### 获得进程PID

方式一：

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# pidof  tmsf-zc
2064
```


方式2：

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# ps   -ef   | grep  tmsf-zc
root      2064     1  0 Mar17 ?        02:24:25 ./tmsf-zc
root      7596  7288  0 15:07 pts/4    00:00:00 grep --color=auto tmsf-zc
root@iZ2ze9qnmldt4l3l82gtviZ:~# 
```



###  查看进程线程个数 及对应PID

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# ps   -efL   | grep  tmsf-zc
root      2064     1  2064  0    5 Mar17 ?        00:00:00 ./tmsf-zc
root      2064     1  2066  0    5 Mar17 ?        01:04:46 ./tmsf-zc
root      2064     1  2067  0    5 Mar17 ?        00:44:53 ./tmsf-zc
root      2064     1  2068  0    5 Mar17 ?        00:34:39 ./tmsf-zc
root      2064     1  2069  0    5 Mar17 ?        00:00:04 ./tmsf-zc
root      7604  7288  7604  0    1 15:08 pts/4    00:00:00 grep --color=auto tmsf-zc
root@iZ2ze9qnmldt4l3l82gtviZ:~# 
```



### 查看进程CPU与内存使用情况 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# top -H   -p 2064

top - 15:10:57 up 14 days,  5:43,  3 users,  load average: 0.00, 0.03, 0.05
Threads:   5 total,   0 running,   5 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.0 sy,  0.0 ni, 99.7 id,  0.3 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   2049904 total,  1961580 used,    88324 free,   204396 buffers
KiB Swap:        0 total,        0 used,        0 free.  1334832 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND                                    
 2064 root      20   0   55752   7180   2756 S  0.0  0.4   0:00.00 tmsf-zc                                    
 2066 root      20   0   55752   7180   2756 S  0.0  0.4  64:46.35 tmsf-zc                                    
 2067 root      20   0   55752   7180   2756 S  0.0  0.4  44:53.83 tmsf-zc                                    
 2068 root      20   0   55752   7180   2756 S  0.0  0.4  34:39.08 tmsf-zc                                    
 2069 root      20   0   55752   7180   2756 S  0.0  0.4   0:04.49 tmsf-zc
```


```
root@iZ2ze9qnmldt4l3l82gtviZ:~# top    -p 2064

top - 15:11:05 up 14 days,  5:43,  3 users,  load average: 0.00, 0.03, 0.05
Tasks:   1 total,   0 running,   1 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.8 us,  1.2 sy,  0.0 ni, 98.0 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   2049904 total,  1961448 used,    88456 free,   204396 buffers
KiB Swap:        0 total,        0 used,        0 free.  1334836 cached Mem

  PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND                                    
 2064 root      20   0   55752   7180   2756 S  0.0  0.4 144:25.09 tmsf-zc   
```


### 查看进程对应的可执行文件 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# ls   -l   /proc/2064/exe   
lrwxrwxrwx 1 root root 0 Mar 17 10:12 /proc/2064/exe -> /test/tmsf-zc
root@iZ2ze9qnmldt4l3l82gtviZ:~# 
```

### 查看进程占用fd个数

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# ls   -l   /proc/2064/fdinfo   | wc   -l
18
```


```
root@iZ2ze9qnmldt4l3l82gtviZ:~# lsof    -p   2064     |  grep  tmsf-zc 
tmsf-zc 2064 root  cwd    DIR  253,1     4096 1967431 /test
tmsf-zc 2064 root  rtd    DIR  253,1     4096       2 /test
tmsf-zc 2064 root  txt    REG  253,1  8660422 1049012 test/tmsf-zc
tmsf-zc 2064 root  mem    REG  253,1  1840928  796801 /lib/x86_64-linux-gnu/libc-2.19.so
tmsf-zc 2064 root  mem    REG  253,1   141574  796793 /lib/x86_64-linux-gnu/libpthread-2.19.so
tmsf-zc 2064 root  mem    REG  253,1   149120  796794 /lib/x86_64-linux-gnu/ld-2.19.so
tmsf-zc 2064 root    0r   CHR    1,3      0t0    5304 /dev/null
tmsf-zc 2064 root    1w   REG  253,1   306118 1971776 test/nohup.out
tmsf-zc 2064 root    2w   REG  253,1   306118 1971776 test/nohup.out
tmsf-zc 2064 root    3u  sock    0,7      0t0  974790 can't identify protocol
tmsf-zc 2064 root    4u  0000    0,9        0    5259 anon_inode
tmsf-zc 2064 root    5u  IPv4 192212      0t0     TCP 172.17.11.187:47222->60.191.13.153:http (ESTABLISHED)
tmsf-zc 2064 root    6u  IPv4 216011      0t0     TCP 172.17.11.187:47275->60.191.13.153:http (ESTABLISHED)
tmsf-zc 2064 root    7u  IPv4 270741      0t0     TCP 172.17.11.187:40874->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root    8u  IPv4 357728      0t0     TCP 172.17.11.187:40966->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root    9u  IPv4 417481      0t0     TCP 172.17.11.187:47589->60.191.13.153:http (ESTABLISHED)
tmsf-zc 2064 root   10u  IPv4 504612      0t0     TCP 172.17.11.187:41185->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root   11u  IPv4 555010      0t0     TCP 172.17.11.187:47795->60.191.13.153:http (ESTABLISHED)
tmsf-zc 2064 root   12u  IPv4 665995      0t0     TCP 172.17.11.187:41409->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root   13u  IPv4 717507      0t0     TCP 172.17.11.187:42001->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root   14u  IPv4 755724      0t0     TCP 172.17.11.187:42108->60.191.13.122:http (ESTABLISHED)
tmsf-zc 2064 root   15u  IPv4 846915      0t0     TCP 172.17.11.187:48714->60.191.13.153:http (ESTABLISHED)
tmsf-zc 2064 root   16u  IPv4 974784      0t0     TCP 172.17.11.187:49598->60.191.13.153:http (ESTABLISHED)
```

### 从proc/pid/stat获取更多进程信息 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# procstat   2064
                 pid: 2064
               tcomm: (tmsf-zc)
               state: S
                ppid: 1
                pgid: 2058
                 sid: 1911
              tty_nr: 0
            tty_pgrp: -1
               flags: 1077960960
             min_flt: 7652
            cmin_flt: 0
             maj_flt: 10
            cmaj_flt: 0
               utime: 2558.590000
               stime: 6106.500000
              cutime: 0.000000
              cstime: 0.000000
            priority: 20
                nice: 0
         num_threads: 5
       it_real_value: 0.000000
          start_time: 03.17 10:12 (971043.78s)
               vsize: 57090048
                 rss: 1795
              rsslim: 9223372036854775807
          start_code: 4194304
            end_code: 7446752
         start_stack: 140735025190528
                 esp: 140735025189968
                 eip: 4549907
             pending: 0000000000000000
             blocked: 0000000000000000
              sigign: 0000000000000003
            sigcatch: 000000007fc1fefc
               wchan: 0
               zero1: 0
               zero2: 0
         exit_signal: 0000000000000011
                 cpu: 0
         rt_priority: 0
              policy: 0
root@iZ2ze9qnmldt4l3l82gtviZ:~#
```

### 查看进程父子关系 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# pstree -g -p  -s   2064 
init(1,1)───tmsf-zc(2064,2058)─┬─{tmsf-zc}(2066,2058)
                               ├─{tmsf-zc}(2067,2058)
                               ├─{tmsf-zc}(2068,2058)
                               └─{tmsf-zc}(2069,2058)
root@iZ2ze9qnmldt4l3l82gtviZ:~# 
```


### 查看进程内存map信息 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# pmap   -x  -p   2064 
2064:   ./tmsf-zc
Address           Kbytes     RSS   Dirty Mode  Mapping
0000000000400000    3180    1472       0 r-x-- /test/tmsf-zc
000000000071b000    2336     948       0 r---- /test/tmsf-zc
0000000000963000     204     104      72 rw--- /test/tmsf-zc
0000000000996000     140      60      60 rw---   [ anon ]
0000000000c6a000     132       4       4 rw---   [ anon ]
000000c000000000       8       8       8 rw---   [ anon ]
000000c41ffc0000     256     256     256 rw---   [ anon ]
000000c420000000    2048    1364    1364 rw---   [ anon ]
000000c420200000    2048    2048    2048 rw---   [ anon ]
000000c420400000    4096     672     672 rw---   [ anon ]
00007f27075d8000    1408     112     112 rw---   [ anon ]
00007f2707738000       4       0       0 -----   [ anon ]
00007f2707739000    8192       8       8 rw---   [ anon ]
00007f2707f39000       4       0       0 -----   [ anon ]
00007f2707f3a000    8192       8       8 rw---   [ anon ]
00007f270873a000       4       0       0 -----   [ anon ]
00007f270873b000    8192       8       8 rw---   [ anon ]
00007f2708f3b000       4       0       0 -----   [ anon ]
00007f2708f3c000    8192       8       8 rw---   [ anon ]
00007f270973c000    1768     232       0 r-x-- /lib/x86_64-linux-gnu/libc-2.19.so
00007f27098f6000    2048       0       0 ----- /lib/x86_64-linux-gnu/libc-2.19.so
00007f2709af6000      16      16      16 r---- /lib/x86_64-linux-gnu/libc-2.19.so
00007f2709afa000       8       8       8 rw--- /lib/x86_64-linux-gnu/libc-2.19.so
00007f2709afc000      20      16      16 rw---   [ anon ]
00007f2709b01000     100      68       0 r-x-- /lib/x86_64-linux-gnu/libpthread-2.19.so
00007f2709b1a000    2044       0       0 ----- /lib/x86_64-linux-gnu/libpthread-2.19.so
00007f2709d19000       4       4       4 r---- /lib/x86_64-linux-gnu/libpthread-2.19.so
00007f2709d1a000       4       4       4 rw--- /lib/x86_64-linux-gnu/libpthread-2.19.so
00007f2709d1b000      16       4       4 rw---   [ anon ]
00007f2709d1f000     140     116       0 r-x-- /lib/x86_64-linux-gnu/ld-2.19.so
00007f2709e6f000     780     376     376 rw---   [ anon ]
00007f2709f3f000       8       8       8 rw---   [ anon ]
00007f2709f41000       4       4       4 r---- /lib/x86_64-linux-gnu/ld-2.19.so
00007f2709f42000       4       4       4 rw--- /lib/x86_64-linux-gnu/ld-2.19.so
00007f2709f43000       4       4       4 rw---   [ anon ]
00007fff6d2d2000     132      16      16 rw---   [ stack ]
00007fff6d3e9000       8       4       0 r-x--   [ anon ]
ffffffffff600000       4       0       0 r-x--   [ anon ]
---------------- ------- ------- ------- 
total kB           55752    7964    5092
```

从上面可以获取以下信息：
1. 进程使用哪些共享库及其版本信息，以及这些共享库在内存中的起始位置
2. 指令起始位置 0000000000400000 
3. 初始化全部
4. RSS 

### 查看进程当前的调用栈 


```
[root@localhost db_proxy_server]# pstack  31002
Thread 22 (Thread 0x7f62dafae700 (LWP 31003)):
#0  0x00000038332e1523 in select () from /lib64/libc.so.6
#1  0x00007f62db1e9125 in apr_sleep () from /usr/lib64/libapr-1.so.0
#2  0x00007f62db716096 in log4cxx::helpers::FileWatchdog::run (data=0x2497a70) at filewatchdog.cpp:76
#3  0x00007f62db7778ce in log4cxx::helpers::Thread::launcher (thread=0x2499c00, data=0x2499be8) at threadcxx.cpp:100
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 21 (Thread 0x7f62da5ad700 (LWP 31008)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c3f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c3e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c3e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 20 (Thread 0x7f62d9bac700 (LWP 31009)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c478) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c468) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c468) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 19 (Thread 0x7f62d91ab700 (LWP 31010)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c4f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c4e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c4e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 18 (Thread 0x7f62d87aa700 (LWP 31011)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c578) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c568) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c568) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 17 (Thread 0x7f62d7da9700 (LWP 31012)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c5f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c5e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c5e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 16 (Thread 0x7f62d73a8700 (LWP 31013)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c678) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c668) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c668) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 15 (Thread 0x7f62d69a7700 (LWP 31014)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c6f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c6e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c6e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 14 (Thread 0x7f62d5fa6700 (LWP 31015)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c778) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c768) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c768) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 13 (Thread 0x7f62d55a5700 (LWP 31016)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c7f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c7e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c7e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 12 (Thread 0x7f62d4ba4700 (LWP 31017)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c878) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c868) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c868) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 11 (Thread 0x7f62d41a3700 (LWP 31018)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c8f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c8e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c8e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 10 (Thread 0x7f62d37a2700 (LWP 31019)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c978) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c968) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c968) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 9 (Thread 0x7f62d2da1700 (LWP 31020)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251c9f8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251c9e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251c9e8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 8 (Thread 0x7f62d23a0700 (LWP 31021)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251ca78) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251ca68) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251ca68) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 7 (Thread 0x7f62d199f700 (LWP 31022)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251caf8) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251cae8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251cae8) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 6 (Thread 0x7f62d0f9e700 (LWP 31023)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251cb78) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251cb68) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251cb68) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 5 (Thread 0x7f62d059d700 (LWP 31024)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251f518) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251f508) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251f508) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 4 (Thread 0x7f62cfb9c700 (LWP 31025)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251f598) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251f588) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251f588) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 3 (Thread 0x7f62cf19b700 (LWP 31026)):
#0  0x0000003833a0b68c in pthread_cond_wait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x00000000005dd0c7 in CThreadNotify::Wait (this=0x251f618) at /home/wubo/LeTalk/server/src/db_proxy_server/../base/Thread.h:62
#2  0x000000000063f7b6 in CWorkerThread::Execute (this=0x251f608) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:44
#3  0x000000000063f754 in CWorkerThread::StartRoutine (arg=0x251f608) at /home/wubo/LeTalk/server/src/base/ThreadPool.cpp:27
#4  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#5  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 2 (Thread 0x7f62ce79a700 (LWP 31027)):
#0  0x0000003833a0ba5e in pthread_cond_timedwait@@GLIBC_2.3.2 () from /lib64/libpthread.so.0
#1  0x000000000063f12c in CCondition::waitTime (this=0x251fec0, nWaitTime=5000) at /home/wubo/LeTalk/server/src/base/Condition.cpp:43
#2  0x00000000005d8fbb in CSyncCenter::doSyncGroupChat (arg=0x0) at /home/wubo/LeTalk/server/src/db_proxy_server/SyncCenter.cpp:299
#3  0x0000003833a07aa1 in start_thread () from /lib64/libpthread.so.0
#4  0x00000038332e8aad in clone () from /lib64/libc.so.6
Thread 1 (Thread 0x7f62db1be820 (LWP 31002)):
#0  0x00000038332e90a3 in epoll_wait () from /lib64/libc.so.6
#1  0x0000000000642f5d in CEventDispatch::StartDispatch (this=0x251f3f0, wait_timeout=10) at /home/wubo/LeTalk/server/src/base/EventDispatch.cpp:365
#2  0x000000000063f5fd in netlib_eventloop (wait_timeout=10) at /home/wubo/LeTalk/server/src/base/netlib.cpp:160
#3  0x00000000005e21ad in main (argc=1, argv=0x7ffd32681a78) at /home/wubo/LeTalk/server/src/db_proxy_server/db_proxy_server.cpp:216
[root@localhost db_proxy_server]# 
```

## 进程故障信息 



### dmesg查找  

```
[root@sqwx1 demo_server]# dmesg   | grep demo_server
demo_server[8425]: segfault at 22 ip 0000000000548b4c sp 00007ffff50281b0 error 4 in demo_server[400000+5c4000]
demo_server[22891]: segfault at 50a09b ip 0000000000562723 sp 00007ffdde9276d0 error 7 in demo_server[400000+647000]
demo_server[26185]: segfault at 52ecdb ip 0000000000562723 sp 00007fff09ca4120 error 7 in demo_server[400000+647000]
```


### 查看进程被谁杀死 

```
root@iZ2ze9qnmldt4l3l82gtviZ:~# cat   /var/log/audit/audit.log     | grep  16135
type=OBJ_PID msg=audit(1491364584.465:11575): opid=16135 oauid=0 ouid=0 oses=35 ocomm="tmsf-zc"
```


## 进程性能分析  

主要有下面两种方式：
1. perf工具  
2. google perf  



## 进程调试 

主要针对linux上gdb调试，可以参考文章，这里不作描述  



## 未完，待续  



