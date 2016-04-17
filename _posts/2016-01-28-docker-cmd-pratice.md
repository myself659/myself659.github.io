---
layout: post
title: docker学习：docker image命令实践
categories: docker
tags: docker

---

搭建了docker环境，就来体验一下Docker，常用docker image命令如下：

### 1. 搜索docker image
```
root@localhost ~]# docker search ubuntu  
NAME                              DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
ubuntu                            Ubuntu is a Debian-based Linux operating s...   2954      [OK]       
ubuntu-upstart                    Upstart is an event-based replacement for ...   58        [OK]       
dorowu/ubuntu-desktop-lxde-vnc    Ubuntu with openssh-server and NoVNC on po...   32                   [OK]
torusware/speedus-ubuntu          Always updated official Ubuntu docker imag...   25                   [OK]
ubuntu-debootstrap                debootstrap --variant=minbase --components...   22        [OK]       
tleyden5iwx/ubuntu-cuda           Ubuntu 14.04 with CUDA drivers pre-installed    18                   [OK]
rastasheep/ubuntu-sshd            Dockerized SSH service, built on top of of...   16                   [OK]
consol/ubuntu-xfce-vnc            Ubuntu container with "headless" VNC sessi...   8                    [OK]
ioft/armhf-ubuntu                 [ABR] Ubuntu Docker images for the ARMv7(a...   7                    [OK]
n3ziniuka5/ubuntu-oracle-jdk      Ubuntu with Oracle JDK. Check tags for ver...   7                    [OK]
nuagebec/ubuntu                   Simple always updated Ubuntu docker images...   4                    [OK]
nickistre/ubuntu-lamp-wordpress   LAMP on Ubuntu with wp-cli installed            3                    [OK]
nimmis/ubuntu                     This is a docker images different LTS vers...   3                    [OK]
maxexcloo/ubuntu                  Docker base image built on Ubuntu with Sup...   2                    [OK]
sylvainlasnier/ubuntu             Ubuntu 15.10 root docker images with commo...   1                    [OK]
isuper/base-ubuntu                This is just a small and clean base Ubuntu...   1                    [OK]
densuke/ubuntu-jp-remix           Ubuntu Linuxの日本語remix風味です                       1                    [OK]
seetheprogress/ubuntu             Ubuntu image provided by seetheprogress us...   1                    [OK]
nickistre/ubuntu-lamp             LAMP server on Ubuntu                           1                    [OK]
teamrock/ubuntu                   TeamRock's Ubuntu image configured with AW...   0                    [OK]
konstruktoid/ubuntu               Ubuntu base image                               0                    [OK]
birkof/ubuntu                     Ubuntu 14.04 LTS (Trusty Tahr)                  0                    [OK]
zoni/ubuntu                                                                       0                    [OK]
esycat/ubuntu                     Ubuntu LTS                                      0                    [OK]
rallias/ubuntu                    Ubuntu with the needful                         0     
```

### 2. 下载image

```
docker pull  
```

### 3. 查看image
```
[root@localhost ~]# docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
ubuntu              latest              d55e68e6cc9c        4 weeks ago         187.9 MB
ubuntu              14.04               d55e68e6cc9c        4 weeks ago         187.9 MB
training/sinatra    latest              f0f4ab557f95        19 months ago       447 MB

```

### 4. 删除image
```

[root@localhost ~]# docker  rm   training/sinatra
Error response from daemon: no such id: training/sinatra
Error: failed to remove containers: [training/sinatra]
[root@localhost ~]# docker rm   f0f4ab557f95
Error response from daemon: no such id: f0f4ab557f95
Error: failed to remove containers: [f0f4ab557f95]
[root@localhost ~]# docker rmi   f0f4ab557f95
Untagged: training/sinatra:latest
Deleted: f0f4ab557f954f3e04177663a3af90e88641bcdcce1f02ac900dbd9768ef4945
Deleted: 79e6bf39f99322cc062a79bec4a09de0dd19cb7f5f735b4b6b7832c04b13bb45
Deleted: ce80548340bb03726d391bb8fa4d134f8418c2fff90be9a7323560debdea9bd2
Deleted: e809f156dc985e07105fdc86ec05eb03eb7aac8636dc210e8595d31b55787f4a
Deleted: bfab314f3b766eddf9778f8dce089f44e84ea028f4a44ce68740dce81a844ec8
Deleted: be88c4c27e80023b6aea82f0f2e15fb21c6f4193fe814e5b58010d356dd7846b
Deleted: 3e76c0a80540a0d36493ae7110796fc92f559a191454e3ac19c1d4c650bdd9e0
Deleted: 511136ea3c5a64f264b78b5433614aec563103b4d4702f3ba7d4d2698e22c158
You have new mail in /var/spool/mail/root
[root@localhost ~]# docker   images
REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
ubuntu              latest              af88597ec24b        6 days ago          187.9 MB
ubuntu              14.04               d55e68e6cc9c        4 weeks ago         187.9 MB
```

### 5. 运行image
```
[root@localhost ~]# docker  run   -i     -t      apache2   
```

### 6.  kill运行的docker image 
```
[root@localhost ~]# docker  ps  
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
68333b272b52        ubuntu              "/bin/bash"         18 minutes ago      Up 18 minutes                           clever_babbage      
0ca2aff5b94b        ubuntu              "bash"              48 minutes ago      Up 48 minutes                           focused_hypatia     
You have new mail in /var/spool/mail/root
[root@localhost ~]# docker   kill   68333b272b52 
68333b272b52
[root@localhost ~]# docker    ps  
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
0ca2aff5b94b        ubuntu              "bash"              54 minutes ago      Up 54 minutes                           focused_hypatia     
```

### 7. 制作image
```
[root@localhost tmp]# more  Dockerfile 
FROM apache2  
RUN  apt-get   install  -y  wget 
[root@localhost tmp]# docker   build    -t wget . 
Sending build context to Docker daemon 2.609 MB
Sending build context to Docker daemon 
Step 0 : FROM apache2
 ---> f5cf247f22af
Step 1 : RUN apt-get   install  -y  wget
 ---> Running in ab3cd326c53c

[root@localhost tmp]# docker   images 
REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
wget                latest              be8bf51f39d5        29 seconds ago      229.1 MB
apache2             latest              f5cf247f22af        5 hours ago         223.8 MB
ubuntu              latest              af88597ec24b        6 days ago          187.9 MB
ubuntu              14.04               d55e68e6cc9c        4 weeks ago         187.9 MB

```

