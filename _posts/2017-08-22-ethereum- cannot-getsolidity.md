---
layout: post
title:   以太坊开发环境
categories:  ethereum
tags:  区块链  ethereum
--- 

# 说明  

主要记录以太坊开发环境过程出现的问题

# getCompilers返回失败

## 问题描述

在搭建以太坊开发环境过程中，出现下面的错误信息：

```
Welcome to the Geth JavaScript console!

instance: Geth/v1.6.7-stable-ab5646c5/darwin-amd64/go1.8.3
 modules: admin:1.0 debug:1.0 eth:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 shh:1.0 txpool:1.0 web3:1.0

> web3.eth.getCompilers() 
Error: The method eth_getCompilers does not exist/is not available
    at web3.js:3104:20
    at web3.js:6191:15
    at web3.js:5004:36
    at <anonymous>:1:1
```

## 问题原因 

geth版本编译不在gopath目录下编译

## 解决方法 

在gopath目录重新编译即可 

```
Michaels-iMac:wallet eric$ geth  attach    ipc://Users/eric/wallet/privchain/geth.ipc  
Welcome to the Geth JavaScript console!

instance: Geth/v1.5.9-stable-a07539fb/darwin/go1.8.3
 modules: admin:1.0 debug:1.0 eth:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 shh:1.0 txpool:1.0 web3:1.0

> web3.eth.getCompilers()
["Solidity"]
> 
```





