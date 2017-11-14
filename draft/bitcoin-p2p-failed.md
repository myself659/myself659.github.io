## 问题描述 

```
2017-11-14 08:03:36 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:37 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:38 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:39 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:40 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:41 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:42 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:43 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:44 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:44 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:45 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:46 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:47 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:48 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:49 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:50 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:51 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:52 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:53 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:54 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:55 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:56 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:57 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:58 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:03:59 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:00 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:01 connect() to 104.236.211.206:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:02 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:02 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:03 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
2017-11-14 08:04:04 connect() to 66.178.182.35:19335 failed after select(): Connection refused (111)
```

## 问题原因

1. 获取节点是错误节点 
2. 获取节点与自身网络连接出现故障 

当然这里也顺带学习一下bitcoin P2P部分设计与实现。


## 解决办法 

在对应的bitcoin.conf文件中添加如下内容，指定一些已被验证的节点
```
addnode=217.16.185.175
addnode=85.214.213.86
addnode=90.252.217.49
addnode=107.170.17.56
addnode=45.33.107.92
```