# 配置 

https://github.com/BitMEX/zcash-bitcore-node/blob/master/docs/services/bitcoind.md 

两种方式  

https://github.com/BitMEX/zcash-bitcore-node  

# litecoin  

支持lite钱包 



# 进程分布 

有哪些进程 对应什么业务

bitcoind --conf=/data/bitcore_testnet/bitcoin.conf --datadir=/data/bitcore_testnet --testnet 

启动参数如何设置 

bitcored 启动了bitcoind 作为子进程启动 

对应代码是如何启动呢


```
root@JBI-Bitcoin-Fullnode:/data/bitcore_testnet/testnet3# netstat   -ntlp      | grep bitcoind 
tcp        0      0 127.0.0.1:28332         0.0.0.0:*               LISTEN      5041bitcoind   
tcp        0      0 0.0.0.0:18332           0.0.0.0:*               LISTEN      5041bitcoind   
tcp        0      0 0.0.0.0:18333           0.0.0.0:*               LISTEN      5041bitcoind   
tcp6       0      0 :::18333                :::*                    LISTEN      5041bitcoind   
```

```
root@JBI-Bitcoin-Fullnode:/data/bitcore_testnet/testnet3# netstat   -ntlp      | grep bitcored
tcp6       0      0 :::3001                 :::*                    LISTEN      5031bitcored   

```
# bitcore 配置
```
root@JBI-Bitcoin-Fullnode:~/bitcore# cat bitcore-node.json 
{
  "network": "testnet",
  "port": 3001,
  "services": [
    "bitcoind",
    "insight-api",
    "insight-ui",
    "web"
  ],
  "servicesConfig": {
    "bitcoind": {
      "spawn": {
        "datadir": "/data/bitcore_testnet",
        "exec": "/root/.nvm/versions/node/v8.1.0/lib/node_modules/bitcore/node_modules/bitcore-node/bin/bitcoind"
      }
    }
  }
}
```

bitcoind 这个服务是如何启动呢？


端口工作有哪些？

```
root@JBI-Bitcoin-Fullnode:/data/bitcore_testnet/testnet3# netstat   -ntlp      | grep node
tcp6       0      0 :::3380                 :::*                    LISTEN      20821/node      
tcp6       0      0 :::3231                 :::*                    LISTEN      20820/node      
tcp6       0      0 :::3232                 :::*                    LISTEN      20826/node 
```

3232为bws的端口 


# p2p 如何获取到区块信息 


# 启动进程与配置代码  

```
var node = new BitcoreNode(fullConfig);

  // setup handlers for uncaught exceptions and ctrl+c
  start.registerExitHandlers(process, node);

  node.on('ready', function() {
    log.info('Bitcore Node ready');
  });

  node.on('error', function(err) {
    log.error(err);
  });

  node.start(function(err) {
    if(err) {
      log.error('Failed to start services');
      if (err.stack) {
        log.error(err.stack);
      }
      start.cleanShutdown(process, node);
    }
  });

  return node;

}
```

```
Node.prototype._startService = function(serviceInfo, callback) {
  var self = this;

  log.info('Starting ' + serviceInfo.name);

  var config;
  if (serviceInfo.config) {
    assert(_.isObject(serviceInfo.config));
    assert(!serviceInfo.config.node);
    assert(!serviceInfo.config.name);
    config = serviceInfo.config;
  } else {
    config = {};
  }

  config.node = this;
  config.name = serviceInfo.name;
  var service = new serviceInfo.module(config);

  self.services[serviceInfo.name] = service;

  service.start(function(err) {
    if (err) {
      return callback(err);
    }

    if (service.getAPIMethods) {
      var methodData = service.getAPIMethods();
      var methodNameConflicts = [];
      methodData.forEach(function(data) {
        var name = data[0];
        var instance = data[1]; 
        var method = data[2];

        if (self[name]) {
          methodNameConflicts.push(name);
        } else {
          self[name] = function() {
            return method.apply(instance, arguments);
          };
        }
      });

      if (methodNameConflicts.length > 0) {
        return callback(new Error('Existing API method(s) exists: ' + methodNameConflicts.join(', ')));
      }
    }

    callback();

  });

};
```

以服务的方式进行启动 

## 如何获取到配置信息 

```
Node.prototype._init = function(config) {
    this.configPath = config.path;
    this.errors = errors;
    this.log = log;

    this.datadir = config.datadir;
    this.network = null;
    this.services = {};
    this._unloadedServices = [];

    this.port = config.port;
    this.https = config.https;
    this.httpsOptions = config.httpsOptions;
    this._setNetwork(config);
};
```

https://github.com/bitpay/bitcore-node/issues/530 

