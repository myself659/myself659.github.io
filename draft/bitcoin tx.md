
# db 

## 表结构 

```
> db.txs.find().limit(1)
{ "_id" : ObjectId("59e8507389e7f588df6e7ae9"), "version" : 3, "createdOn" : 1508397171, "id" : "adaae856-e559-4680-967a-dc39937e3294", "walletId" : "596ed25b-39dd-44db-8594-f3a7670416eb", "creatorId" : "fe556bde9c6d7480d2ed0a6a708bc0bdf4fe33dcad039166608eb726315bc059", "coin" : "btc", "network" : "testnet", "outputs" : [ { "amount" : 100000, "toAddress" : "mvpeKNG3FZXBavicW8Pj2mkjUwN4e6gtpT", "message" : null } ], "amount" : 100000, "message" : "{\"iv\":\"MAvpoN/neMfNiKQ4Oi9NBw==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"PVC4Twy5H4DFUiGByNMQiCnjyDmz\"}", "payProUrl" : null, "changeAddress" : { "version" : "1.0.0", "createdOn" : 1508397171, "address" : "myk2iHNqjbuv74MTx5Hs5rxmYXH6SVJPk6", "walletId" : "596ed25b-39dd-44db-8594-f3a7670416eb", "isChange" : true, "path" : "m/1/0", "publicKeys" : [ "03ec58c470764ef8e72551141bf905999c588e5307c2c300df339d2c80363f9c96" ], "coin" : "btc", "network" : "testnet", "type" : "P2PKH", "hasActivity" : null, "_id" : ObjectId("59e85073f6fc637638bf2e4a") }, "inputs" : [ { "txid" : "9af49ae993073452f45f3d0a09225370a3b682b3c996b7e265be9448fbe956f5", "vout" : 0, "address" : "myVGh5R2qdVhs71hCwcN2aHDFHWdmpoNhQ", "scriptPubKey" : "76a914c520ca07d685a3d3e4b7fb5e5b20f40d414e35ea88ac", "satoshis" : 1000000, "confirmations" : 1, "locked" : false, "path" : "m/0/5", "publicKeys" : [ "02ee5de5df9993238642f39e1562b5bc45d334bb3a7e6a95064c7c21a1f9b9bf4b" ] } ], "walletM" : 1, "walletN" : 1, "requiredSignatures" : 1, "requiredRejections" : 1, "status" : "broadcasted", "txid" : "ae67dd1fa4070f7064d3e292ae6634003144f441b30e3a991d73ce72c74867a1", "broadcastedOn" : 1508397171, "inputPaths" : [ "m/0/5" ], "actions" : [ { "version" : "1.0.0", "createdOn" : 1508397171, "copayerId" : "fe556bde9c6d7480d2ed0a6a708bc0bdf4fe33dcad039166608eb726315bc059", "type" : "accept", "signatures" : [ "3045022100fe00eaaa7e203650a8461203eac875dda7ff80af305dcb927c7464f23956c33b0220363647714cfa20ebb1fb876392dc92cc7422ba64cb38e3edef80f1035156e49a" ], "xpub" : "tpubDC2r5q7SKdN5sHJcuA3WtB9o8oQLK2UDVLBs6KtxwzF8K5Z42SnBjw1dCv97DwiGTJDDrgD8eZ3vhtMLJXxhzpHZHtP4DDA3Hsy4cQ6TGYR", "comment" : null, "copayerName" : "{\"iv\":\"JRKdLP24oqQmo/XNN2+MOg==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"RXpN0QAklouXWhSw\"}" } ], "outputOrder" : [ 0, 1 ], "fee" : 12150, "feeLevel" : null, "feePerKb" : 50000, "excludeUnconfirmedUtxos" : false, "addressType" : "P2PKH", "customData" : null, "proposalSignature" : "3045022100ccf5e80f9acd871fd48190e1ad1eb4b9b4fb576dd45f7df7763f0802949494d7022034a6c7d96b23914c0ed6c2daa7ae4b8c8334a54fea5736d2e9c5865114709351", "proposalSignaturePubKey" : null, "proposalSignaturePubKeySig" : null, "derivationStrategy" : "BIP44", "creatorName" : "{\"iv\":\"JRKdLP24oqQmo/XNN2+MOg==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"RXpN0QAklouXWhSw\"}", "raw" : "0100000001f556e9fb4894be65e2b796c9b382b6a3705322090a3d5ff452340793e99af49a000000006b483045022100fe00eaaa7e203650a8461203eac875dda7ff80af305dcb927c7464f23956c33b0220363647714cfa20ebb1fb876392dc92cc7422ba64cb38e3edef80f1035156e49a012102ee5de5df9993238642f39e1562b5bc45d334bb3a7e6a95064c7c21a1f9b9bf4bffffffff02a0860100000000001976a914a7e25e092e0adbbd75e36c389549aaa719d9ba8788ac2a8c0d00000000001976a914c7eb5bfee642f7293322958aa81241b05d1f671c88ac00000000", "note" : null, "isPending" : false }

```

# 确认 

https://www.buybitcoinworldwide.com/confirmations/ 

https://en.bitcoin.it/wiki/Confirmation 

在交易被广播到比特币网络之后，它可能被包含在发布到网络的块中。 当发生这种情况时，据说这笔交易已经被开采了1块的深度。
随着每个后来的块被发现，块的数量增加一个。 为了防止双重支出，一个交易不应该被视为确认，直到它是一定数量的块深度。

经典的比特币客户端将显示交易为“n / unconfirmed”，直到交易达到6个街区。
接受比特币作为付款的商家和交易所可以并且应该设置自己的门槛，以确定需要多少个区块，直到资金被确认为止。
当由于名义上的双重支出而导致的潜在损失（如非常廉价或不可互换的物品）时，人们可以选择不等待交易得到确认，
并且一旦在网络上看到交易就完成交易。大多数交易所和其他商家承担双重开支的风险，需要6个或更多的块。

默认的，经常被引用的6个区块的数字没有什么特别之处。
它是基于这样的假设而选择的，即攻击者不可能累计超过10％的哈希率，并且可以接受小于0.1％的可忽略的风险。
然而，这两个数字都是任意的。对于偶然的攻击者来说，6个区块是过度杀伤性的，同时对于更多的攻击者来说，
攻击者拥有超过10％的hashrate。[1]

新鲜开采的硬币不能用于100块。建议等待一些额外的时间，以便交易将由所有节点传播的更好机会。
一些较老的比特币客户端将不会显示生成的硬币，直到它们达到120块深为止。

https://en.bitcoin.it/wiki/Irreversible_Transactions    不可逆转的交易 

# 状态  

数据用来描述  

描述方式有多种方式？

有哪些变化  

# 交易广播是如何发送的？ 

# 交易确认数是如何计算的 


# 交易确认数是如何获取的 

