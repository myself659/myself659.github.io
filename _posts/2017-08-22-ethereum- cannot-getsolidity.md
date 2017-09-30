---
layout: post
title:   以太坊开发环境问题记录
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

# parity启动失败 

## 问题描述  

搭建测试环境中出现以下问题：

```
root@ia:~#parity --chain=kovan --jsonrpc-hosts=all 
parity: /usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `CXXABI_1.3.8' not found (required by parity)
parity: /usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.20' not found (required by parity)
parity: /usr/lib/x86_64-linux-gnu/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by parity)
```
## 问题原因 

gcc/g++版本是4.8版本 

## 解决方案

升级gcc/g++

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.9 g++-4.9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9
```

# parity访问出现bad gateway问题  

## 问题描述   

```
 curl   -X POST  http://localhost:8545  -H "Content-Type: application/json"  -d  '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'
```
本地访问ok 
```
curl   -X POST  http://47.96.45.67:8545  -H "Content-Type: application/json"  -d  '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'    
```
远程访问返回失败 

## 问题原因 

```
parity --chain=kovan --jsonrpc-hosts=all
```
上面命令限定侦听端口为本机接口 

## 解决方案 

```
parity --chain=kovan   --jsonrpc-interface=0.0.0.0  --jsonrpc-hosts=all
```
以上述命令启动parity打开侦听限制 



# 没有eth无法在kovan上部署智能合约

## 问题描述  

没有eth无法部署成功部署智能合约 

## 问题原因 

天下没有免费的午餐 

## 解决方法

参考[Kovan Faucet](https://github.com/kovan-testnet/faucet),四种方式选择其中一个即可，Github Gist Faucet方式参考[这里](https://gist.github.com/myself659/5c803a89aa725a64206fb775e00a0815)  



