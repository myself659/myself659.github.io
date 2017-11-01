# promise 

promise如何理解？

异步编程的一个封装？

语法形式：

reslove 

reject 

then 

## 示例 

```
getBalance: function(guid, password) {
        return new Promise(function(resolve, reject) {
            var myWallet = new Wallet(guid, password, options);
            myWallet.getBalance().then(function(balance) {
                    return resolve(balance);
                })
                .catch(function(err) {
                    return reject(err);
                });
        });
    }
```

```
 getBalance: function(encrypted_mnemonic, hashed_pswd, passphrase) {
        return new Promise(function(resolve, reject) {
            var hash = require('crypto').createHash('sha256');
            var key = hash.update(hashed_pswd).digest('base64');
            var mnemonic = aes256.decrypt(key, encrypted_mnemonic);
            //client.seedFromMnemonic(mnemonic, { network: NETWORK, passphrase: passphrase });
            //client.openWallet(function(err, wal) {
            client.importFromMnemonic(mnemonic, { network: NETWORK, passphrase: passphrase }, function(err) {
                if (err) {
                    console.log(err);
                    return reject(err);
                }
                client.getBalance(function(err, balance) {
                    if (err) {
                        console.log(err);
                        return reject(err);
                    }
                    var Balance = {
                        available: balance.availableAmount,
                        totalAmount: balance.totalAmount
                    };
                    //console.log(balance);
                    return resolve(Balance);
                });
            });
        });
    }
```
# npm 

http://www.ruanyifeng.com/blog/2016/01/npm-install.html

## npm view jquery versions 

## npm init  

## npm  install  

## npm clean  

