---
layout: post
title: Linux netstat 应用示例
categories: Linux 
tags: shell, network
 
---
### 关于netstat ###
netstat 命令用于显示各种网络相关信息，如网络连接，路由表，接口状态 (Interface Statistics)，masquerade 连接，多播成员 (Multicast Memberships) 等等。

### 常用参数 ###
-a (all)显示所有选项，默认不显示LISTEN相关

-t (tcp)仅显示tcp相关选项

-u (udp)仅显示udp相关选项

-n 拒绝显示别名，能显示数字的全部转化成数字

-l 仅列出有在 Listen (监听) 的服務状态

-p 显示建立相关链接的程序名

-r 显示路由信息，路由表

-e 显示扩展信息，例如uid等

-s 按各个协议进行统计

-c 每隔一个固定时间，执行该netstat命令


注意：LISTEN和LISTENING的状态只有用-a或者-l才能看到

### 应用实例 ###
#### 1. 选项组合应用 ####
命令： netstat  -tlnp

说明：显示处于listen状态的tcp连接，并显示对应进程pid

示例：

```
 [root@localhost default]# netstat  -tlnp 

Active Internet connections (only servers)

Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name 
 
tcp        0      0 0.0.0.0:21                  0.0.0.0:*                   LISTEN      1284/pure-ftpd 

tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      1022/sshd   
       
tcp        0      0 0.0.0.0:445                 0.0.0.0:*                   LISTEN      1309/smbd 
         
tcp        0      0 0.0.0.0:139                 0.0.0.0:*                   LISTEN      1309/smbd   
       
tcp        0      0 0.0.0.0:6379                0.0.0.0:*                   LISTEN      1298/redis-server 

tcp        0      0 0.0.0.0:11211               0.0.0.0:*                   LISTEN      1002/memcached  
   
tcp        0      0 0.0.0.0:80                  0.0.0.0:*                   LISTEN      31604/nginx  
      
tcp        0      0 :::21                       :::*                        LISTEN      1284/pure-ftpd 

tcp        0      0 :::22                       :::*                        LISTEN      1022/sshd    
```

#### 2. 显示网关地址 ####
命令： netstat  -rn  | grep  UG  | tr  -s " "  | cut  -d  " "  -f2

说明：先显示路由信息，找出网关所对应表项，删除多余的空格并显示第二个表项field 

示例：
```
[root@localhost default]# netstat  -rn   | grep  UG  | tr -s  " " | cut -d  " " -f2

192.168.20.1
```

#### 3.统计tcp各种连接状态的个数 ####
命令：
netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}
说明：显示连接信息，通过awk统计各tcp状态连接个数

示例：
```
[root@localhost default]#netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}

ESTABLISHED 516
TIME_WAIT 14
```

#### 4.显示所有tcp监听端口 ####
命令：
netstat -lnt | awk '{print $4}' | cut -f2 -d: | grep -o '[0-9]*'
说明：

示例：显示处于listen状态的tcp连接，打印每行第4个域元素，并以:作为该域的内部分隔符，同时显示其中第二个域元素，对结果进行过滤，只显示数字部分
```
[root@localhost default]# netstat -lnt  | awk '{print $4}' | cut -f2 -d: | grep -o '[0-9]*'

21
22
445
139
6379
11211
80
```

#### 5.统计每个IP连接数 ####
命令：
netstat -anp |grep 'tcp\|udp' | awk '{print $5}' | sed s/::ffff:// | cut -d: -f1 | sort | uniq -c | sort -n 
说明：显示所有网络连接并从中过滤出tcp与udp连接，打印这些连接表项的第5个域元素，删除包含/::ffff:的表项，以：为分隔符显示剩下的内容的第一个域元素，再进行排序并统计个数，最后以数值排列显示结果 


示例：

```
[root@localhost default]# netstat -anp |grep 'tcp\|udp' | awk '{print $5}' | sed s/::ffff:// | cut -d: -f1 | sort | uniq -c | sort -n

3 192.168.70.36

8 0.0.0.0
```

#### 6.查看80端口的连接，并排序 ####
命令：netstat -ant | grep “:80″ | grep ESTABLISHED | awk ‘{printf “%s %s\n”,$5,$6}’ | sort

说明：
不解释了，你懂的

示例：



未完，待续
