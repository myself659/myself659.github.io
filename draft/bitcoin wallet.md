
# 创建钱包流程 

```
/Users/eric/go/src/gitlab.chainresearch.org/wallet/bitcore-wallet-client/bitcore-wallet-client.js
```


# client代码 

```
API.prototype.createWallet = function(walletName, copayerName, m, n, opts, cb) {
  var self = this;

  if (!self._checkKeyDerivation()) return cb(new Error('Cannot create new wallet'));

  if (opts) $.shouldBeObject(opts);
  opts = opts || {};

  var network = opts.network || 'livenet';
  if (!_.contains(['testnet', 'livenet'], network)) return cb(new Error('Invalid network'));

  if (!self.credentials) {
    log.info('Generating new keys');
    self.seedFromRandom({
      network: network
    });
  } else {
    log.info('Using existing keys');
  }

  if (network != self.credentials.network) {
    return cb(new Error('Existing keys were created for a different network'));
  }

  var walletPrivKey = opts.walletPrivKey || new Bitcore.PrivateKey();

  var c = self.credentials;
  c.addWalletPrivateKey(walletPrivKey.toString());
  var encWalletName = Utils.encryptMessage(walletName, c.sharedEncryptingKey);

  var args = {
    name: encWalletName,
    m: m,
    n: n,
    pubKey: (new Bitcore.PrivateKey(walletPrivKey)).toPublicKey().toString(),
    network: network,
    singleAddress: !!opts.singleAddress,
    id: opts.id,
  };
  self._doPostRequest('/v2/wallets/', args, function(err, res) {
    if (err) return cb(err);

    var walletId = res.walletId;
    c.addWalletInfo(walletId, walletName, m, n, copayerName);
    var secret = API._buildSecret(c.walletId, c.walletPrivKey, c.network);

    self._doJoinWallet(walletId, walletPrivKey, c.xPubKey, c.requestPubKey, copayerName, {},
      function(err, wallet) {
        if (err) return cb(err);
        return cb(null, n > 1 ? secret : null);
      });
  });
};

```

# service 代码  

/Users/eric/go/src/gitlab.chainresearch.org/wallet/bitcore-wallet-service/lib/expressapp.js

```
router.post('/v2/wallets/', createWalletLimiter, function(req, res) {
    var server;
    try {
      server = getServer(req, res);
    } catch (ex) {
      return returnError(ex, res, req);
    }
    server.createWallet(req.body, function(err, walletId) {
      if (err) return returnError(err, res, req);
      res.json({
        walletId: walletId,
      });
    });
  });
```

防止恶意创建钱包 


# DB

检查钱包是否存在 

```
function(acb) {
      var wallet = Wallet.create({
        id: opts.id,
        name: opts.name,
        m: opts.m,
        n: opts.n,
        coin: opts.coin,
        network: opts.network,
        pubKey: pubKey.toString(),
        singleAddress: !!opts.singleAddress,
        derivationStrategy: derivationStrategy,
        addressType: addressType,
      });
      self.storage.storeWallet(wallet, function(err) {
        log.debug('Wallet created', wallet.id, opts.network);
        newWallet = wallet;
        return acb(err);
      });
    }
```


数据设计与实现 

/Users/eric/go/src/gitlab.chainresearch.org/wallet/bitcore-wallet-service/lib/storage.js 


数据库初始化 

index.js 会有一些初始化代码  


## 表结构
id是如何生成的？


```
{ "_id" : ObjectId("59e6fa8289e7f588df6e7a8a"), "version" : "1.0.0", "createdOn" : 1508309634, "id" : "36fd76f7-f0cf-4ccb-9ddc-0500323c0689", "name" : "{\"iv\":\"A5dPZwz6ShXFlN1iuFuwDQ==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"DeCr9058NuHo9JM5fNXUlbg=\"}", "m" : 1, "n" : 1, "singleAddress" : false, "status" : "complete", "publicKeyRing" : [ { "xPubKey" : "tpubDCdkHqsHDF74gc6AdxNgC4czMWexVfHt9syLpz1mLxTV2Dk4CWMnjJdXvg8KfRURz7M9YAVUxeGBxq8JtnzZnERbUtENTmpgwLPPietPfKQ", "requestPubKey" : "039fedb44f3edb8147422ad2f15c713221d609e1ec623f2585d80d7c29f6912dfc" } ], "copayers" : [ { "version" : 2, "createdOn" : 1508309634, "coin" : "btc", "xPubKey" : "tpubDCdkHqsHDF74gc6AdxNgC4czMWexVfHt9syLpz1mLxTV2Dk4CWMnjJdXvg8KfRURz7M9YAVUxeGBxq8JtnzZnERbUtENTmpgwLPPietPfKQ", "id" : "54cfacd75647d92fc8e763c4cb08ad6c29d1d8834705c2a42439be6031cca203", "name" : "{\"iv\":\"4Rd5G75oBDWK0VHlpW2Kgg==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"gSCugPwRxoN/f2yCkQ==\"}", "requestPubKey" : "039fedb44f3edb8147422ad2f15c713221d609e1ec623f2585d80d7c29f6912dfc", "signature" : "304402202b42a0507a87c0f1819ec2d9ad7d8d87b5e00fbe37a12119a13b8104382a95f4022015d09cbff61307b2ba73ac5e471e91dec0df51aeb14146e681a5832f192df381", "requestPubKeys" : [ { "key" : "039fedb44f3edb8147422ad2f15c713221d609e1ec623f2585d80d7c29f6912dfc", "signature" : "304402202b42a0507a87c0f1819ec2d9ad7d8d87b5e00fbe37a12119a13b8104382a95f4022015d09cbff61307b2ba73ac5e471e91dec0df51aeb14146e681a5832f192df381" } ], "customData" : "{\"iv\":\"D90IvvhGHve9wA7YbMy+tA==\",\"v\":1,\"iter\":1,\"ks\":128,\"ts\":64,\"mode\":\"ccm\",\"adata\":\"\",\"cipher\":\"aes\",\"ct\":\"S8Mvi/JdT548HBd8l0RKXKnB14jtNDX/O6osXbUMhnv++2T/d4rikVzKEMuCD860/4wjPwVhhAyRBmGInRILGPwWwlCl36JVmADzYI9GEz+9fymntmUPK2w5gzc=\"}" } ], "pubKey" : "039f3c36741f1e7bbbd06f338ed46e557ccca519e0817906869439569ffbb30b3f", "coin" : "btc", "network" : "testnet", "derivationStrategy" : "BIP44", "addressType" : "P2PKH", "addressManager" : { "version" : 2, "derivationStrategy" : "BIP44", "receiveAddressIndex" : 0, "changeAddressIndex" : 0, "copayerIndex" : 2147483647 }, "scanStatus" : null, "isShared" : false }

```

db.wallets.


# 进程环境  

```
/data/bitcore_testnet/bitcoin.conf
server=1
whitelist=127.0.0.1
txindex=1
addressindex=1
timestampindex=1
spentindex=1
zmqpubrawtx=tcp://127.0.0.1:28332
zmqpubhashblock=tcp://127.0.0.1:28332
rpcallowip=127.0.0.1
rpcuser=bitcoin
rpcpassword=local321
uacomment=bitcore

```
配置
