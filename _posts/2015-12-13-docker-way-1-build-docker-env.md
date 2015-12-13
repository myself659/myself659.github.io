---
layout: post
title: docker学习：升级Linux内核，搭建docker环境
categories: docker
tags: docker

---

docker可以说是去年最热的技术，也是业界大谈特谈的技术，到了今年有很多公司已经将docker应用于自己的生产环境。作为一名程序员应该与时俱进，学习新技术，不断地提高自己。

### 升级内核
docker要求linux内核版本3.12以上，作为常用linux2.26.32版本，虽然也可以安装docker，但是有一些特性不支持，所以第一步就是升级内核。个人选择最新的长期维护版本升级linux4.13,内核升级最关键的一件事情就是配置内核，关于内核支持docker的内核配置文件，如果不想自己动手配置内核，可以参考[Linux-4.13-configfordocker](https://github.com/myself659/docker-way/blob/master/env/linux-4.1.3-configfordocker);

在未正确配置linux内核会出现以下一些错误：

1. docker启动过程中iptables命令执行失败，原因是 iptables模块没有配置

```
[root@localhost ~]# docker -d
INFO[0000] Listening for HTTP on unix (/var/run/docker.sock) 
INFO[0000] [graphdriver] using prior storage driver "devicemapper" 
FATA[0000] Error starting daemon: Error initializing network controller: Error creating default "bridge" network: Failed to Setup IP tables: Unable to enable NAT rule: iptables failed: iptables -t nat -I POSTROUTING -s 172.17.42.1/16 ! -o docker0 -j MASQUERADE: iptables v1.4.7: can't initialize iptables table `nat': Table does not exist (do you need to insmod?)
Perhaps iptables or your kernel needs to be upgraded.
 (exit status 3) 
You have new mail in /var/spool/mail/root
[root@localhost ~]# 
```

2. cgroup创建失败，具体参考[stackoverflow](http://stackoverflow.com/questions/25183063/docker-on-rhel-6-cgroup-mounting-failing)

```
[root@localhost ~]# service docker start 
Starting cgconfig service: Error: cannot mount memory to /cgroup/memory: No such file or directory
/sbin/cgconfigparser; error loading /etc/cgconfig.conf: Cgroup mounting failed
Failed to parse /etc/cgconfig.conf or /etc/cgconfig.d[FAILED]
```

具体内核升级过程参考如下：

```
[root@localhost linux-4.1.13]# cp /share/github/docker-way/env/linux-4.1.3-configfordocker   .config 
[root@localhost linux-4.1.13]# sh  -c   'yes  "" | make oldconfig'
[root@localhost linux-4.1.13]# make   -j2 bzImage 
[root@localhost linux-4.1.13]# make   -j2  modules 
[root@localhost linux-4.1.13]# make   -j2   modules_install  
[root@localhost linux-4.1.13]# make install

```

检查grub配置，修改并将linux-4.13作为默认启动项，重启系统，系统启动成功后，查看内核版本：

```
[root@localhost ~]# uname   -a
Linux localhost.localdomain 4.1.13 #1 SMP Sat Dec 5 11:17:50 CST 2015 x86_64 x86_64 x86_64 GNU/Linux
```


### 安装device-mapper

```
[root@localhost ~]#yum install -y device-mapper

```

### 安装docker

```
[root@localhost ~]# yum   -y   install docker-io  
```

安装完成后，检查docker是否能够正确启动

```
[root@localhost ~]# docker  -d
INFO[0000] [graphdriver] using prior storage driver "devicemapper" 
INFO[0000] Listening for HTTP on unix (/var/run/docker.sock) 
WARN[0000] Your kernel does not support swap memory limit. 
INFO[0000] Loading containers: start.                   

INFO[0000] Loading containers: done.                    
INFO[0000] Daemon has completed initialization          
INFO[0000] Docker daemon                                 commit=786b29d execdriver=native-0.2 graphdriver=devicemapper version=1.7.1
```

没有错误，接下来可以开启一段docker学习之旅了!

<end>
