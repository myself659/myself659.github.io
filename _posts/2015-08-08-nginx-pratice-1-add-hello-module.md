---
layout: post
title: nginx实践：添加自定义模块hello
categories: nginx
tags: nginx

---
nginx是一个值得学习与研究的开源代码，写这篇文章主要目的是让自己能够能够从最简单的任务开始，通过写作促进自己一步一步地深入学习与分析nginx。本文是这个系列的第一篇，主要是记录自己实现一个自定义的模块hello的过程。

### 1. 下载源码 
下载nginx 1.8.0 源代码

```
root@localhost github]# wget  http://nginx.org/download/nginx-1.8.0.tar.gz
root@localhost github]# tar zxf  
```

### 2. 准备文件与代码 
在nginx解压目录下，添加如下文件：

```
[root@localhost nginx-1.8.0]# tree | more
.
|-- **addon**
|   `-- **hello**
|       |-- **config**
|       `-- **ngx_http_hello_module.c**
|-- auto
|   |-- cc
|   |   |-- acc
|   |   |-- bcc
```

新增文件分析参考nginx-hello

```
[root@localhost nginx-1.8.0]# grep -r  ngx_addon_name  /share/github/nginx-1.8.0
/share/github/nginx-1.8.0/auto/modules:            echo " + $ngx_addon_name was configured"
/share/github/nginx-1.8.0/addon/hello/config:ngx_addon_name=ngx_http_hello_module
[root@localhost nginx-1.8.0]# 

[root@localhost nginx-1.8.0]# grep -r  add-module   /share/github/nginx-1.8.0
/share/github/nginx-1.8.0/auto/options:        --add-module=*)                  NGX_ADDONS="$NGX_ADDONS $value" ;;
/share/github/nginx-1.8.0/auto/options:  --add-module=PATH                  enable an external module
```

### 3. 编译与安装 
编译三步走 

```
1. ./configure   --add-module=/share/github/nginx-1.8.0/addon/hello

2. make  

3. make install 
```

### 4.测试
在/usr/local/nginx/conf/nginx.conf文件的http配置项下，添加如下内容：

```
location =/hello{
                    hello;
            }
```


过程如下：

```
[root@localhost ~]# nginx -t
nginx: the configuration file /usr/local/nginx/conf/nginx.conf syntax is ok
nginx: configuration file /usr/local/nginx/conf/nginx.conf test is successful
[root@localhost ~]# service nginx  restart
Restarting nginx (via systemctl):  [  OK  ]
[root@localhost nginx-1.8.0]# curl http://localhost/hello/
hello nginx!
```

另外也可以通过浏览器访问：http://serverip/hello

如果出现不能打开，检查一下iptable 设置，在/etc/sysconfig/iptables增加下面一条配置，允许80端口通过：

-A INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT 


然后输入命令service iptables  restart重启iptables服务，使新增配置生效




### 5. 实例代码 ###
本文代码请参考：[nginx-hello](https://github.com/myself659/nginx-hello)

本人水平有限，若有疏漏与错误，欢迎交流与指正。



