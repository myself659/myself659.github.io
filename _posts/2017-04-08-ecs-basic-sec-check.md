---
layout: post
title:  阿里云ECS基本安全检查小结 
categories:  Secure
tags:  云计算  安全
--- 

### 背景 

查看阿里云ECS服务器
```
type=USER_AUTH msg=audit(1491669519.156:15631): pid=22938 uid=0 auid=4294967295 ses=4294967295 msg='op=PAM:authentication acct="deploy" exe="/usr/sbin/sshd" hostname=123.57.245.163 addr=123.57.245.163 terminal=ssh res=failed'
type=USER_LOGIN msg=audit(1491669519.156:15632): pid=22938 uid=0 auid=4294967295 ses=4294967295 msg='op=login acct=28696E76616C6964207573657229 exe="/usr/sbin/sshd" hostname=? addr=123.57.245.163 terminal=sshd res=failed'
type=USER_LOGIN msg=audit(1491669587.368:15634): pid=22940 uid=0 auid=4294967295 ses=4294967295 msg='op=login acct=28756E6B6E6F776E207573657229 exe="/usr/sbin/sshd" hostname=? addr=123.57.245.163 terminal=sshd res=failed'
type=USER_LOGIN msg=audit(1491669587.368:15635): pid=22940 uid=0 auid=4294967295 ses=4294967295 msg='op=login acct=28696E76616C6964207573657229 exe="/usr/sbin/sshd" hostname=? addr=123.57.245.163 terminal=sshd res=failed'
type=USER_AUTH msg=audit(1491669589.420:15636): pid=22940 uid=0 auid=4294967295 ses=4294967295 msg='op=PAM:authentication acct="deploy" exe="/usr/sbin/sshd" hostname=123.57.245.163 addr=123.57.245.163 terminal=ssh res=failed'
type=USER_LOGIN msg=audit(1491669589.420:15637): pid=22940 uid=0 auid=4294967295 ses=4294967295 msg='op=login acct=28696E76616C6964207573657229 exe="/usr/sbin/sshd" hostname=? addr=123.57.245.163 terminal=sshd res=failed'
type=USER_LOGIN msg=audit(1491669658.624:15639): pid=22948 uid=0 auid=4294967295 ses=4294967295 msg='op=login acct=28756E6B6E6F776E207573657229 exe="/usr/sbin/sshd" hostname=? addr=123.57.245.163 terminal=sshd res=failed'
```

百度一下这个ip地址，如下图所示：
![记下这个IP,留下证据](../image/attck ip.png)

这是有人在阿里云ECS写脚本要暴力破解云主机的密码。这个引起我的注意，赶紧检查一下自己的ECS是否被破解。

说点题外话：记得去年帮人弄一个网站的时候，后面的她的云主机密码泄漏了，人家第一反应过来就是怀疑我，小白用户还得花心思给她解释有多种情况会导致密码泄漏与破解，伤不起啊。

整个检查大体如下，分为以下几个部分：

1. 检查有哪些用户？
2. 用户会干什么？
3. 用户干过什么？ 


## 检查用户 

### 查看当前在线用户 

```
root@ecs-1:~# who 
```

为了确定是否当前就有不明用户登录，如果有不明用户登录，那么得赶紧修改密码。

### 查看当前ssh连接情况 

```
root@ecs-1:~# ss  -t   sport = :22   
```

查看用户的ssh连接情况，进行对比确认 

### 查看新增用户

```
root@ecs-1:~# awk -F':' '{ print $1}' /etc/passwd
```


### 查看当前用户 

```
root@ecs-1:~# awk -F':' '{ print $1}' /etc/passwd
```

主要是确定是否存在可疑用户，是否有可疑用户加入 

### 检查是否存在特权用户 

```
root@ecs-1:~# awk -F: '$3==0 {print $1}' /etc/passwd
root
```

### 检查用户口令是否为空 


```
root@ecs-1:~# awk -F: 'length($2)==0 {print $1}' /etc/shadow
```



## 检查用户会干什么 


### top查看内存与内存使用情况 

```
root@ecs-1:~# top  
```

关注高cpu使用率与高内存占用程序，例如比特币的矿机一般会占用大量的cpu资源  


### 查看任务计划 

```
root@ecs-1:~# crontab   -l
```

### 查看网络连接情况 

```
root@ecs-1:~# netstat   -natp  
```

### 检查隐藏进程  

检查隐藏进程，三步走：

第一步： 
```
root@ecs-1:~# ps  -ef    | awk   '{print $2}'  | sort  -n    |  uniq   > pid1
```

第二步：

```
root@ecs-1:~# ls   /proc  | grep -E '[0-9]{1,}' | sort   -n    | uniq   > pid2  
```

第三步：

```
root@ecs-1:~# diff pid1 pid2  
1d0
< PID
155,158c154,157
< 29592
< 29593
< 29594
< 29595
---
> 29588
> 29589
> 29590
> 29591
```

结果说明： 第一步有4个不同的进程表示这条命令对应的进程，第二步同样的，所以这里没有发现隐藏的进程 

### 检查rcx.d 

```
root@ecs-1:~# ls /etc/rc3.d
```

## 检查用户干过什么 

重点检查以下情况：
1. 用户登录 
2. 机器重启情况
3. 安全日志 
4. 历史操作 
5. 安全文件检查 

###  用last -d 查看登录情况 

```
root@ecs-1:~# last   -d  
```



### 用last  -x 查看机器重启情况 

```
root@ecs-1:~# last   -x reboot  
```

### 查看audit的log信息 

```
root@ecs-1:~# cat   /var/log/audit/audit.log  
```


### 查看历史命令  

```
root@ecs-1:~# history    
  271  iptables   -L
  272  package main
```



### 安全文件检查  

1. 检查授权的ssh key，查看是否有未知的ssh key添加 

```
root@ecs-1:~#  cat   ~/.ssh/authorized_keys       
```

2. 检查bashrc

```
root@ecs-1:~#  cat   ~/.bashrc      
```

3. 检查密码最后修改时间 

```
root@ecs-1:~# ls -l /etc/passwd
```


## 其他 

### 网络隔离性 

网络采用了VPC隔离检测结果如下：
```
root@ecs-1#  nmap -v -sT 172.17.11.0/28   --open

Starting Nmap 6.40 ( http://nmap.org ) at 2017-03-14 10:23 CST
Initiating ARP Ping Scan at 10:23
Scanning 16 hosts [1 port/host]
Completed ARP Ping Scan at 10:23, 0.41s elapsed (16 total hosts)
Initiating Parallel DNS resolution of 16 hosts. at 10:23
Completed Parallel DNS resolution of 16 hosts. at 10:24, 13.00s elapsed
Initiating Connect Scan at 10:24
Scanning 16 hosts [1000 ports/host]
Connect Scan Timing: About 2.42% done; ETC: 10:45 (0:20:49 remaining)
Connect Scan Timing: About 11.94% done; ETC: 10:46 (0:19:40 remaining)
Connect Scan Timing: About 16.72% done; ETC: 10:46 (0:18:31 remaining)
Connect Scan Timing: About 21.57% done; ETC: 10:46 (0:17:20 remaining)
Connect Scan Timing: About 26.57% done; ETC: 10:46 (0:16:13 remaining)
Connect Scan Timing: About 31.72% done; ETC: 10:46 (0:15:06 remaining)
Connect Scan Timing: About 37.01% done; ETC: 10:46 (0:13:59 remaining)
Connect Scan Timing: About 41.86% done; ETC: 10:46 (0:12:52 remaining)
Connect Scan Timing: About 47.34% done; ETC: 10:46 (0:11:45 remaining)
Connect Scan Timing: About 52.56% done; ETC: 10:46 (0:10:37 remaining)
Connect Scan Timing: About 57.65% done; ETC: 10:46 (0:09:27 remaining)
Connect Scan Timing: About 62.67% done; ETC: 10:46 (0:08:19 remaining)
Connect Scan Timing: About 67.69% done; ETC: 10:46 (0:07:11 remaining)
Connect Scan Timing: About 72.92% done; ETC: 10:46 (0:06:01 remaining)
Connect Scan Timing: About 78.18% done; ETC: 10:46 (0:04:51 remaining)
Connect Scan Timing: About 83.26% done; ETC: 10:46 (0:03:43 remaining)
Connect Scan Timing: About 88.35% done; ETC: 10:46 (0:02:35 remaining)
Connect Scan Timing: About 93.58% done; ETC: 10:46 (0:01:25 remaining)
Completed Connect Scan against 172.17.11.14 in 1276.55s (15 hosts left)
Completed Connect Scan against 172.17.11.7 in 1279.69s (14 hosts left)
Completed Connect Scan against 172.17.11.1 in 1280.09s (13 hosts left)
Completed Connect Scan against 172.17.11.10 in 1291.35s (12 hosts left)
Completed Connect Scan against 172.17.11.12 in 1291.68s (11 hosts left)
Completed Connect Scan against 172.17.11.5 in 1322.08s (10 hosts left)
Completed Connect Scan against 172.17.11.4 in 1322.13s (9 hosts left)
Completed Connect Scan against 172.17.11.15 in 1325.48s (8 hosts left)
Completed Connect Scan against 172.17.11.3 in 1328.10s (7 hosts left)
Completed Connect Scan against 172.17.11.9 in 1329.43s (6 hosts left)
Completed Connect Scan against 172.17.11.11 in 1331.76s (5 hosts left)
Completed Connect Scan against 172.17.11.2 in 1335.78s (4 hosts left))

Completed Connect Scan against 172.17.11.0 in 1336.84s (3 hosts leftCompleted Connect Scan against 172.17.11.6 in 1337.46s (2 hosts left)
Completed Connect Scan against 172.17.11.8 in 1338.91s (1 host left)
Completed Connect Scan at 10:46, 1342.34s elapsed (16000 total ports)
Read data files from: /usr/bin/../share/nmap
Nmap done: 16 IP addresses (16 hosts up) scanned in 1355.80 seconds
           Raw packets sent: 16 (448B) | Rcvd: 16 (448B)
```


如果网络没有隔离，各个VM同一个局域网内，会发现同一个网段的开放的端口，检查结果如下：
```
root@centos# nmap   -v -sT  192.168.20.0/24 --open 

Discovered open port 80/tcp on 192.168.20.10
Discovered open port 3389/tcp on 192.168.20.63
Discovered open port 80/tcp on 192.168.20.38
Discovered open port 135/tcp on 192.168.20.49
Discovered open port 3389/tcp on 192.168.20.52
Discovered open port 80/tcp on 192.168.20.49
Discovered open port 80/tcp on 192.168.20.34
Discovered open port 445/tcp on 192.168.20.37
Discovered open port 445/tcp on 192.168.20.36
Discovered open port 135/tcp on 192.168.20.14
Discovered open port 3389/tcp on 192.168.20.38
Discovered open port 135/tcp on 192.168.20.50
```


通过对比可知，采用VPC隔离的ECS不能发现同一个网段开放的网络端口，这样安全性也大大增加，好在阿里云ECS已经支持上VPC,这样就不担心来自同一个网段的攻击了 




## 总结   

1. 安全首先是一个意识问题，需要保证足够的敏锐 
2. 阿里云的ip基本是一个大段范围，这么多IP对应的ECS肯定有些用户是安全小白用户，这样攻击者的成本也不会很高，写一个脚本遍历这个大的ip池，就可以获取一些小白用户的ECS
3. 密码一定不要用12345678之类或者那些常用的密码，另外注意定时更换密码 
4. 对阿里云一个建议：对于暴力破解ssh密码，增加功能实现同一个ip地址多次尝试ssh的限制 
5. 这篇主要写一些基本检查操作，有空的时候再写一篇加强安全防护的文章。 

不足与建议，欢迎指正与交流。

（end）


### 参考 

1.  [Understanding Audit Log Files](https://access.redhat.com/documentation/en-US/Red_Hat_Enterprise_Linux/6/html/Security_Guide/sec-Understanding_Audit_Log_Files.html)
2.  linux man  
3.  [Hiding Linux Processes For Fun And Profit](https://sysdig.com/blog/hiding-linux-processes-for-fun-and-profit/)
4.  [Linux: Hide Processes From Other Users](https://www.cyberciti.biz/faq/linux-hide-processes-from-other-users/)
5.  [Understanding Red Hat Run Levels](http://www.linuxjournal.com/article/1274)
