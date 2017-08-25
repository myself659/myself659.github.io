---
layout: post
title:   以太坊私链智能合约实践  
categories:  ethereum
tags:  区块链  ethereum
--- 


# 环境说明

- 操作系统: macos.10.12.1 
- geth版本：1.5.9 
- solc版本：0.4.15  

# 实践  

## 1. 启动本地geth节点 

```
Michaels-iMac:wallet eric$ geth   --rpc --rpcaddr 127.0.0.1 --rpcport 8545 --dev --datadir myethchain  
```
如上操作后，geth  console输出如下：

```
I0824 17:28:57.448455 p2p/server.go:340] Starting Server
I0824 17:28:59.554115 p2p/discover/udp.go:227] Listening, enode://04697f62537244ee34aea28e613530a1f46a64de75d8174d963c9ca0c2e6b96d4aa756ef7a33e269de1b7c088163835b72dda8f4dea712cf39569db4e8d8e43a@[::]:54798
I0824 17:28:59.554264 p2p/server.go:608] Listening on [::]:58245
I0824 17:28:59.554324 whisper/whisperv2/whisper.go:176] Whisper started
I0824 17:28:59.570668 node/node.go:341] IPC endpoint opened: /Users/eric/wallet/myethchain/geth.ipc
I0824 17:28:59.581423 node/node.go:411] HTTP endpoint opened: http://127.0.0.1:8545
```

同时也创建在当前目录创建目录myethchain，其结构如下：

```
Michaels-iMac:wallet eric$ tree  myethchain/
myethchain/
├── geth
│   ├── LOCK
│   ├── chaindata
│   │   ├── 000002.log
│   │   ├── CURRENT
│   │   ├── LOCK
│   │   ├── LOG
│   │   └── MANIFEST-000003
│   ├── nodekey
│   └── nodes
│       ├── 000001.log
│       ├── CURRENT
│       ├── LOCK
│       ├── LOG
│       └── MANIFEST-000000
├── geth.ipc
└── keystore

4 directories, 13 files
```

##  2. 连接geth节点 

在另外一个终端里连接geth节点。

```
Michaels-iMac:wallet eric$ geth  attach   ipc://Users/eric/wallet/myethchain/geth.ipc  
Welcome to the Geth JavaScript console!

instance: Geth/v1.5.9-stable-a07539fb/darwin/go1.8.3
 modules: admin:1.0 debug:1.0 eth:1.0 miner:1.0 net:1.0 personal:1.0 rpc:1.0 shh:1.0 txpool:1.0 web3:1.0

> 
```

## 3. 确保solidity编译器安装正确 

```
> web3.eth.getCompilers()
["Solidity"]
> 
```
如果出错或者获取为空，请检查solc安装情况。 

## 4. 查看帐户列表 

```
> personal.listAccounts
null
> 
```

## 5. 创建帐户 

```
> personal.newAccount('pw1234')
"0xaf3139393f2ecb2455cbc2c7639ce111017fb90b"
> 
```

## 6. 查看帐户列表

```
> personal.listAccounts
["0xaf3139393f2ecb2455cbc2c7639ce111017fb90b"]
> 
```

## 7. 查看coibase地址 

```
> web3.eth.coinbase 
"0xaf3139393f2ecb2455cbc2c7639ce111017fb90b"
```

与上面地址一致，默认取第一个创建帐户的地址，作为挖矿的收益打入该地址 

## 8. 准备智能合约 

```
> source = "contract hello { function multen(uint a) returns(uint d){return a * 10;}}"
"contract hello { function multen(uint a) returns(uint d){return a * 10;}}"
> 
```

## 9. 编译智能合约 

```
> code = web3.eth.compile.solidity(source)
{
  <stdin>:hello: {
    code: "0x60606040523415600e57600080fd5b5b60978061001d6000396000f300606060405263ffffffff7c0100000000000000000000000000000000000000000000000000000000600035041663e847973c8114603c575b600080fd5b3415604657600080fd5b604f6004356061565b60405190815260200160405180910390f35b600a81025b9190505600a165627a7a72305820395c2030cb020d0b4f79ac0803f1aa28b97082d962a13f37a914f7950e1de5ec0029",
    info: {
      abiDefinition: [{...}],
      compilerOptions: "--combined-json bin,abi,userdoc,devdoc --add-std --optimize",
      compilerVersion: "0.4.15",
      developerDoc: {
        methods: {}
      },
      language: "Solidity",
      languageVersion: "0.4.15",
      source: "contract hello { function multen(uint a) returns(uint d){return a * 10;}}",
      userDoc: {
        methods: {}
      }
    }
  }
}
```


### 10. 根据ABI创建合约对象

```
> MyContract = web3.eth.contract(code["<stdin>:hello"].info.abiDefinition)
{
  abi: [{
      constant: false,
      inputs: [{...}],
      name: "multen",
      outputs: [{...}],
      payable: false,
      type: "function"
  }],
  eth: {
    accounts: ["0xaf3139393f2ecb2455cbc2c7639ce111017fb90b"],
    blockNumber: 0,
    coinbase: "0xaf3139393f2ecb2455cbc2c7639ce111017fb90b",
    compile: {
      lll: function(),
      serpent: function(),
      solidity: function()
    },
    defaultAccount: undefined,
    defaultBlock: "latest",
    gasPrice: 21782200000,
    hashrate: 0,
    mining: false,
    pendingTransactions: [],
    protocolVersion: "0x3f",
    syncing: false,
    call: function(),
    contract: function(abi),
    estimateGas: function(),
    filter: function(fil, callback),
    getAccounts: function(callback),
    getBalance: function(),
    getBlock: function(),
    getBlockNumber: function(callback),
    getBlockTransactionCount: function(),
    getBlockUncleCount: function(),
    getCode: function(),
    getCoinbase: function(callback),
    getCompilers: function(),
    getGasPrice: function(callback),
    getHashrate: function(callback),
    getMining: function(callback),
    getPendingTransactions: function(callback),
    getProtocolVersion: function(callback),
    getRawTransaction: function(),
    getRawTransactionFromBlock: function(),
    getStorageAt: function(),
    getSyncing: function(callback),
    getTransaction: function(),
    getTransactionCount: function(),
    getTransactionFromBlock: function(),
    getTransactionReceipt: function(),
    getUncle: function(),
    getWork: function(),
    iban: function(iban),
    icapNamereg: function(),
    isSyncing: function(callback),
    namereg: function(),
    resend: function(),
    sendIBANTransaction: function(),
    sendRawTransaction: function(),
    sendTransaction: function(),
    sign: function(),
    signTransaction: function(),
    submitTransaction: function(),
    submitWork: function()
  },
  at: function(address, callback),
  getData: function(),
  new: function()
}
> 

```

## 11. 挖矿

第一步：获取帐户地址
 
 ```
 > account1 = web3.eth.coinbase
"0xaf3139393f2ecb2455cbc2c7639ce111017fb90b"
> 
 ```
第二步：查看balance

```
> web3.eth.getBalance(account1)
0
> 
```
第三步：启动挖矿 

```
> miner.start() 
true
```

第四步：查看balance

```
> web3.eth.getBalance(account1)
424531250000000000000
```

第五步: 停止挖矿 

钱差不多就够了。 

```
> miner.stop()
true
> 
```

## 12. 解锁帐户 

目的是为了发送交易  

```
> personal.unlockAccount(account1, 'pw1234')
true
```

## 13. 初始化智能合约 

```
> bytecode = code["<stdin>:hello"].code
"0x60606040523415600e57600080fd5b5b60978061001d6000396000f300606060405263ffffffff7c0100000000000000000000000000000000000000000000000000000000600035041663e847973c8114603c575b600080fd5b3415604657600080fd5b604f6004356061565b60405190815260200160405180910390f35b600a81025b9190505600a165627a7a72305820395c2030cb020d0b4f79ac0803f1aa28b97082d962a13f37a914f7950e1de5ec0029"
> 
```

获取部署合约的gas费用开销
```
> web3.eth.estimateGas({data: bytecode})
30886
> 
```

## 14. 部署智能合约 

```
contractInstance = MyContract.new({data: bytecode gas: 1000000, from: account1}, function(e, contract){if(!e){if(!contract.address){console.log("Contract transaction send: Transaction Hash: "+contract.transactionHash+" waiting to be mined...");}else{console.log("Contract mined! Address: "+contract.address);console.log(contract);}}else{console.log(e)}})  
Contract transaction send: Transaction Hash: 0xeeac0f028e559d469b94805b986b1d9a0bd0d30289e3285487310ca426dae034 waiting to be mined...
{
  abi: [{
      constant: false,
      inputs: [{...}],
      name: "multen",
      outputs: [{...}],
      payable: false,
      type: "function"
  }],
  address: undefined,
  transactionHash: "0xeeac0f028e559d469b94805b986b1d9a0bd0d30289e3285487310ca426dae034"
}
```

如果出现以下错误：
```
> contractInstance = MyContract.new({data: bytecode gas: 1000000, from: account1}, function(e, contract){if(!e){if(!contract.address){console.log("Contract transaction send: Transaction Hash: "+contract.transactionHash+" waiting to be mined...");}else{console.log("Contract mined! Address: "+contract.address);console.log(contract);}}else{console.log(e)}})
Error: authentication needed: password or unlock
{
  abi: [{
      constant: false,
      inputs: [{...}],
      name: "multen",
      outputs: [{...}],
      payable: false,
      type: "function"
  }],
  address: undefined,
  transactionHash: null
}
```
重新解锁用户即可


## 14. 重新挖矿 

```
> miner.start() 
true
> Contract mined! Address: 0x58dba5bddccde639cef014c1766561abbc46c13f
[object Object]
```
## 15. 切换 console 

```
> Contract mined! Address: 0x58dba5bddccde639cef014c1766561abbc46c13f
[object Object]
^C
>
```

## 16. 检查合约安装是否成功 

```
> eth.getCode(contractInstance.address)
"0x606060405263ffffffff7c0100000000000000000000000000000000000000000000000000000000600035041663e847973c8114603c575b600080fd5b3415604657600080fd5b604f6004356061565b60405190815260200160405180910390f35b600a81025b9190505600a165627a7a72305820395c2030cb020d0b4f79ac0803f1aa28b97082d962a13f37a914f7950e1de5ec0029"
> 
```

## 17. 调用合约函数  

```
> contractInstance.multen.call(9)  
90
```

(end)


