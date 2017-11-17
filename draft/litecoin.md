
# http://testnet.litecointools.com/  

从测试网络获取测试币

测试地址
mn4QRtUyWker5km9BxjYDEWwATWWc7nhB7


mvsQb3GTNMLbB1gUGZSBNMxetK6C9c7wwV

```
RESPONSE:
{
    "amount": -10,
    "fee": -0.000748,
    "confirmations": 0,
    "trusted": true,
    "txid": "336baec48a4deeca49f97815f468587240620f94f0a201b3ceca99ecb7245a07",
    "walletconflicts": [],
    "time": 1509586666,
    "timereceived": 1509586666,
    "bip125-replaceable": "no",
    "details": [
        {
            "account": "",
            "address": "mqSyupPJHoT7L5BnbbsW68ZKJwajVYPupt",
            "category": "send",
            "amount": -10,
            "vout": 1,
            "fee": -0.000748,
            "abandoned": false
        }
    ],
    "hex": "010000000210de8ebf46d6d331f6e0b96da8f6723f9eaefa8662d6af4d801fd774061864c7010000006b48304502210081af2857f267583b68b7043078f72b36c612fbfb23e4e1004a706fc0cc26c9a302207ac05aa5fa086a4a5f975e64653e380de596663f2c7a980434524ab874a5fdf2012103cb7d746261dea3726607d9f0b9213fb12503033bc5411f91dd3f9855c2f862d3feffffff04d2da4c9d0f54d573fb8f9a4da376f021efa8883e45d9ea63fdfee3ffa7dbba010000006b483045022100cc2695e3c57a60ed66e1ad1a7bda24c4a5b8fec9c064939497945a55fa25434f02201b30dd57f7321f0c90e283642f8291c390987447e76a1c6a52d8428d0c9a993f012102c6dd31cfa6c6b6b87a76e7e2fbf587a89586447b28255a97852832eb38ec93b9feffffff023caa1100000000001976a9144b011eb02b7e22c2cda7d396246cc6e20c9dea1888ac00ca9a3b000000001976a9146cf0f18787df65825880a46034242f9d85019f7988ac11cd0300"
}
```
对应url是多少？

http://explorer.litecointools.com/address/mqSyupPJHoT7L5BnbbsW68ZKJwajVYPupt 


## 怎么获取区块高度 


# 创建测试网络 

litecore-node create -d   _litenode-data  -t  testnet 

litecore-node create -d   _ltc-data  -t  ltc-test 

# 更改mongodb的数据库名

```

```

# More than one instance of bitcore 

https://github.com/bitpay/bitcore/issues/1188

npm list 查看有哪些包 

包冲突了怎么办？

# 3001端口启动失败 

http://192.168.15.139:3001/insight/


没有启动成功 

怎么启动？

```
npm install -g litecore-node
litecore-node create mynode
cd mynode
litecore-node install insight-lite-api
litecore-node install insight-lite-ui
litecore-node start

litecore-node create -d <bitcoin-data-dir> mynode
cd mynode
litecore-node install <service>

```

```
jbi@192-168-15-139:~/testnet$ litecore-node install   insight-api
npm notice created a lockfile as package-lock.json. You should commit this file.
+ insight-api@0.4.3
added 94 packages in 127.283s
Successfully added services(s): insight-api
jbi@192-168-15-139:~/testnet$ litecore-node install   insight-ui
+ insight-ui@0.4.0
added 1 package in 29.549s
Successfully added services(s): insight-ui
```

# Error querying the blockchain 

```
http://192.168.15.139:3001/api/addrs/utxo

http://192.168.15.139:3001/insight-api/addrs/utxo
```

```
WARN Insight http://localhost:3001/api/addrs/utxo Returned Status: 404
ERR! /v1/balance/?r=34212 :500:Error querying the blockchain
::ffff:192.168.15.249 2017-11-02T07:19:17.079Z "GET /bws/api/v1/balance/?r=34212" 500 41 3.603 "node-superagent/3.8.0" d99eec83-1aad-4077-b951-5d7a8f2d2d26 cf7b15b5d45528865aff936ba7b7ef3a34e948fe8f9c637bd16c5e5ad7850a42
```




# litecore-node  

WARN Insight http://localhost:3001/api/addrs/utxo Returned Status: 404


https://github.com/bitpay/bitcore-wallet-client/issues/176


# api 

## block 获取block信息 

http://192.168.15.139:3001/insight-api/block/4bc8d0f5895d24c3786231001a835216f1d3d98d3efaa2d1ab30b34265ea0c34


## uxto 


/bws/api/v1/balance/??twoStep=1&r=24861 


/insight-api/addr/[:addr]/utxo

http://192.168.15.139:3001/insight-api/addr/mqSyupPJHoT7L5BnbbsW68ZKJwajVYPupt/utxo 

测试ok 

那么问题的原因在哪里呢？

没有设置前缀

```
  var providerOptions = {
    provider: 'insight',
    url: (self.node.https ? 'https://' : 'http://') + 'localhost:' + self.node.port,
    apiPrefix: '/insight-api'
  };
```

## getAddressActivity 

http://192.168.15.139:3001/insight-api/addr/mqSyupPJHoT7L5BnbbsW68ZKJwajVYPupt


# client-api 

## tx-history 

336baec48a4deeca49f97815f468587240620f94f0a201b3ceca99ecb7245a07

# UNMET DEPENDENCY litecore-node@^3.1.1 

npm ERR! missing: litecore-node@^3.1.11, required by testnet

# Can't connect to bitcoind to get live updates from the p2p network. (Tried connecting to bitcoind at : and failed.)

与内网有关  
不能够在连接p2p节点 
需要公网ip地址

导致的问题是获取block失败 
```
curl  http://192.168.15.139:3001/insight-api/blocks?limit=5 
curl: (52) Empty reply from server
```


# 环境搭建  


##  litecore-node 


npm install -g litecore-node 

## 创建节点 



# litecoind  

19335 用于侦听P2P端口  

# 存储空间 

## 容量

```
jbi@192-168-15-139:~/_litenode-data$ df   -h   /home/jbi/.litecoin/
Filesystem                      Size  Used Avail Use% Mounted on
/dev/mapper/localhost--vg-root   45G   25G   18G  59% /
```

## 升级空间

如何操作？

更换节点 

影响服务 没有影响  

内存  

level db 存储的哪里 

monodb 是用本地 ？还是用阿里云  


# bcmonitor

bcmonitor功能 获取新的交易与区块 
这个服务必须要启动成功 


# livenet 测试 

```
 curl http://127.0.0.1:3001/insight-api/tx/6d911185209372575f6db3d2c63ee62fed35053d9eae0cb489a9422502ef3d8c
```

# Invalid or missing peers.dat; recreating  


https://bitcointalk.org/index.php?topic=2169215.0

https://github.com/bitcoin/bitcoin/commit/b86a42077a134888c53bfd406b87bf0a39c78264 


# 线上环境  

https://ltc-insight.58wallet.io/insight  

http://47.52.232.123:3001/insight

http://localhost:3001/insight

https://ltc.58wallet.io/bws/api/v2/feelevels/ 


http://47.52.232.123:3232/bws/api/v2/feelevels/ 


https://ltc-insight.58wallet.io/insight/

https://ltc-test.58wallet.io/bws/api/v2/feelevels/ 

https://ltc-insight-test.58wallet.io/insight/