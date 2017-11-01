

# addresses  

```
{ "_id" : ObjectId("59e96223f6fc637638bf2e59"), "version" : "1.0.0", "createdOn" : 1508467235, "address" : "muTZ8JL2xNydnU3G8fL1w9qqGqYnp6Hafo", "walletId" : "596ed25b-39dd-44db-8594-f3a7670416eb", "isChange" : true, "path" : "m/1/1", "publicKeys" : [ "03e20670d349e0d2ccbbca553b42d104e14e84a080c8217f5ab4636eb5b3a508cd" ], "coin" : "btc", "network" : "testnet", "type" : "P2PKH", "hasActivity" : null }
```

## 相关命令

db.addresses.find()

db.addresses.find({},{"walletId":"596ed25b-39dd-44db-8594-f3a7670416eb"})


db.addresses.find({"walletId":"596ed25b-39dd-44db-8594-f3a7670416eb"})

测试使用的钱包id：
596ed25b-39dd-44db-8594-f3a7670416eb

对应是wallet的id 
这两个表联系起来了 




## 如何生成walletid 

创建钱包的一些流程 

如何同wallet表联系起来的 ？

