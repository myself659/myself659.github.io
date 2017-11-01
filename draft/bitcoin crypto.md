
# keystore形式 

```
{
    "coin":"btc",
    "network":"testnet",
    "xPrivKey":"tprv8ZgxMBicQKsPeAgVrzCYdz8Q4qEkRjQiokKm58t7Z3rjN1JhnrWxmjJBL7zwykS3pGdP1Tk3Yyb3FvmjVRpnbP8ypn6wwxxke5vFJed1C2f",
    "xPubKey":"tpubDC2r5q7SKdN5sHJcuA3WtB9o8oQLK2UDVLBs6KtxwzF8K5Z42SnBjw1dCv97DwiGTJDDrgD8eZ3vhtMLJXxhzpHZHtP4DDA3Hsy4cQ6TGYR",
    "requestPrivKey":"b4cb7c040447a5a623fffef2066211b1031b64035927de048a49010c2af196db",
    "requestPubKey":"0299ac4f2a2a491c2fde25a9ba701b6fac56a2bd6a8d43b70c9d676d63554c4ca2",
    "copayerId":"fe556bde9c6d7480d2ed0a6a708bc0bdf4fe33dcad039166608eb726315bc059",
    "publicKeyRing":[
        {
            "xPubKey":"tpubDC2r5q7SKdN5sHJcuA3WtB9o8oQLK2UDVLBs6KtxwzF8K5Z42SnBjw1dCv97DwiGTJDDrgD8eZ3vhtMLJXxhzpHZHtP4DDA3Hsy4cQ6TGYR",
            "requestPubKey":"0299ac4f2a2a491c2fde25a9ba701b6fac56a2bd6a8d43b70c9d676d63554c4ca2"
        }
    ],
    "walletId":"596ed25b-39dd-44db-8594-f3a7670416eb",
    "walletName":"bitwallet",
    "m":1,
    "n":1,
    "walletPrivKey":"ce7da32ea4c4f98689e1d5af5e86017827a163a3efdab0fa38acb14d93d2199c",
    "personalEncryptingKey":"ZSZmA48O511vbk9buob5Jw==",
    "sharedEncryptingKey":"wsmsZ3wxrn3LIZQOBqqEKQ==",
    "copayerName":"eric",
    "entropySource":"e8529000882a6a13841f52e94526c46c3e172a973ed3e6fe8e42be0aad1ebf4e",
    "derivationStrategy":"BIP44",
    "account":0,
    "compliantDerivation":true,
    "addressType":"P2PKH"
}
```

各个字段的解释 

如何生成助记词？

# xPrivKey 

BIP32相关 HD钱包
https://github.com/bitcoin/bips/blob/master/bip-0032.mediawiki


https://github.com/WeMeetAgain/go-hdwallet 

go语言实现参考 

这个是主私钥  用于恢复帐号  


# requestPrivKey 

两个来源 从hd wallet 与 xPriv 

主要用于什么呢？

用于钱包帐户登录与认证  

# walletPrivKey 


```
  var walletPrivKey = opts.walletPrivKey || new Bitcore.PrivateKey();
```
这一个私钥太重要了 

公钥基于这个私钥生成 
secret由这个私钥生成 
可以画一张图来说明



# walletId 

## 如何生成walletId 

从创建代码开始

非共享钱包可以前端生成与设置，如果没有则由后端自成 

共享钱包不可以生成 

```
    var secretSplit = split(secret, [22, 74, 75]);
    var widBase58 = secretSplit[0].replace(/0/g, '');
    var widHex = Bitcore.encoding.Base58.decode(widBase58).toString('hex');
    var walletId = split(widHex, [8, 12, 16, 20]).join('-');
```

下面是测试代码，
```
var walletId = Uuid.v4();
```

# entropySource 


# copayer相关 


共享钱包 
多人进行签名 
需要共享的是什么信息 

```
 var secret = Client._buildSecret(walletId, walletPrivKey, coin, network);
```


```
Wallet Created. Share this secret with your copayers: Sdzu32KkjZbXdTrjxKUgsyL4WBeUdPG3VsNiBngCdAz49bLWrjzWBREkphQy1AwHH56tVo17HfTbtc
```


## copayerid 生成 

```
this.copayerId = Utils.xPubToCopayerId(this.coin, this.xPubKey);

```






# 加密算法 

https://github.com/btcsuite/btcd/tree/master/btcec 

学习这个就可以了 
结合代码学习 
写一篇文章就可以了

fabric也有一些代码 
fabric CA相关代码 
