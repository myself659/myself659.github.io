# 创世block 

```
 configtxgen -profile TwoOrgsOrdererGenesis -outputBlock ./channel-artifacts/genesis.block
```

configtxgen 对应的代码是由什么生成？


```
configtxgen` is a tool that allows for the creation of a genesis block using profiles, or grouped configuration parameters — refer to the [Configuring using the connfigtxgen tool](../docs/source/configtxgen.rst) guide for more.

```

https://hyperledger-fabric.readthedocs.io/en/latest/configtxgen.html?highlight=configtxgen

基本的例子  

# 代码位置 
```
Michaels-iMac:fabric eric$ find  .  -name   configtxgen  
./common/configtx/tool/configtxgen
```