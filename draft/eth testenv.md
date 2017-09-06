# 测试环境  

https://github.com/kovan-testnet/rpc-node-setup  

https://github.com/kovan-testnet/config  

# 获取eth的帐本

通过api一个一个获取，解析block信息  

block的编码方式  


https://github.com/paritytech/parity/issues/3225  


https://ethereum.stackexchange.com/questions/9877/parity-command-failes-due-to-missing-c-libs/9878 

https://stackoverflow.com/questions/20357033/how-to-fix-program-name-usr-lib-x86-64-linux-gnu-libstdc-so-6-version-cxx


http://47.93.49.72:8545  

解决这个问题升级gcc版本  解决问题  stackoverflow 解决问题  


bad gatway   

本地测试 

curl   -X POST  http://localhost:8545  -H "Content-Type: application/json"  -d  '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'


内网访问是没有错的


curl   -X POST  http://47.93.49.72:8545  -H "Content-Type: application/json"  -d  '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'


```
# curl   -X POST  http://47.93.49.72:8545  -H "Content-Type: application/json"  -d  '{"jsonrpc":"2.0","method":"eth_getBlockByNumber","params":["0x1b4", true],"id":1}'
curl: (7) Failed to connect to 47.93.49.72 port 8545: Connection refused
```

nginx 反向代理是必须的  

搭建节点是必要的 

用户在不同的地方访问 很导致一些问题  nginx反向代理   


iptables -A INPUT -p tcp --dport 8545 -j ACCEPT 


parity --chain=kovan --jsonrpc-hosts=all   --rpcaddr=47.93.49.72  --rpcport=8545

--rpcaddr IP                     Equivalent to --jsonrpc-interface IP.
--rpcport PORT                   Equivalent to --jsonrpc-port PORT.


parity --chain=kovan   --jsonrpc-interface=0.0.0.0  --jsonrpc-hosts=all


## 全节点内容  

https://etherchain.org/block/4240137 

一个块的内容  


