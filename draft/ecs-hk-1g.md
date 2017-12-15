# 作为数据盘 

mysql 

mongodb 

## mysql

https://www.digitalocean.com/community/tutorials/how-to-install-mysql-on-ubuntu-16-04


## mongodb  

写入速度  

https://www.digitalocean.com/community/tutorials/how-to-install-mongodb-on-ubuntu-16-04 

https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-mongodb-on-ubuntu-16-04 

自己下载代码进行安装的方式  


https://stackoverflow.com/questions/37372684/mongodb-3-2-authentication-failed  

# ipds.top  

```
Michaels-iMac:examples eric$ dig +trace   ipds.top  

; <<>> DiG 9.8.3-P1 <<>> +trace ipds.top
;; global options: +cmd
.			176691	IN	NS	h.root-servers.net.
.			176691	IN	NS	f.root-servers.net.
.			176691	IN	NS	g.root-servers.net.
.			176691	IN	NS	b.root-servers.net.
.			176691	IN	NS	e.root-servers.net.
.			176691	IN	NS	c.root-servers.net.
.			176691	IN	NS	a.root-servers.net.
.			176691	IN	NS	j.root-servers.net.
.			176691	IN	NS	k.root-servers.net.
.			176691	IN	NS	l.root-servers.net.
.			176691	IN	NS	m.root-servers.net.
.			176691	IN	NS	i.root-servers.net.
.			176691	IN	NS	d.root-servers.net.
;; Received 228 bytes from 114.114.114.114#53(114.114.114.114) in 377 ms

top.			172800	IN	NS	a.zdnscloud.com.
top.			172800	IN	NS	b.zdnscloud.com.
top.			172800	IN	NS	c.zdnscloud.com.
top.			172800	IN	NS	d.zdnscloud.com.
top.			172800	IN	NS	e.zdnscloud.com.
top.			172800	IN	NS	f.zdnscloud.com.
top.			172800	IN	NS	g.zdnscloud.com.
top.			172800	IN	NS	i.zdnscloud.com.
top.			172800	IN	NS	j.zdnscloud.com.
;; Received 351 bytes from 193.0.14.129#53(193.0.14.129) in 390 ms

ipds.top.		3600	IN	NS	dns3.hichina.com.
ipds.top.		3600	IN	NS	dns4.hichina.com.
;; Received 75 bytes from 1.8.242.1#53(1.8.242.1) in 61 ms

ipds.top.		600	IN	A	47.52.105.103
;; Received 42 bytes from 140.205.41.13#53(140.205.41.13) in 27 ms

Michaels-iMac:examples eric$ 

```
http://www.jianshu.com/p/252b542b1abf  
修改域名设置，进行个性化设置 

f1g1ns1.dnspod.net.
f1g1ns2.dnspod.net.

域名解析 得到的数据可以利用起来 

