
# umengpusher  

主要功能是从NSQ接收通知（消息），然后将通知（消息）通过umeng API发送到umeng服务器，后续由umeng完成推送通知（消息）到手机。 


## Key信息

```
var (
	// Version version
	Version       = "0.0.1"
	app           = kingpin.New("app", "umeng push server").DefaultEnvars()
	conf          = UmengConfig{}
	androidAppKey = "59cdfcac99f0c737c7000174"
	androidSecret = "cdobqkjkycu9qmaggwvjl9aamlmcm4on"
	iOSAppKey     = "59cdfcff04e20509990000a5"
	iOSSecret     = "itt4vjr4ugedem7vezzya33ksb7xxqzb"
)
```

具体代码参考：https://gitlab.chainresearch.org/wallet/stone/blob/develop/cmd/umengpusher/main.go

## 参考  

1. http://bbs.umeng.com/thread-20339-1-1.html 
2. http://bbs.umeng.com/thread-13706-1-1.html 
3. http://dev.umeng.com/push/android/api-doc 



# bitfinexrecorder  

主要功能抓取bitfinex交易历史信息，历史信息保存到mysql数据库。
数据库信息参考代码：https://gitlab.chainresearch.org/wallet/stone/blob/develop/service/web/bitfinextrade.go

```
//TableName TableName
func (t *TradeHist) TableName() string {
	return t.MktCode + "_" + t.MktPair + "_" + t.KType
}
```

# coincapworker  

主要功能抓取coincap行情信息，行情信息保存到mysql数据库。
数据库信息参考代码：
https://gitlab.chainresearch.org/wallet/stone/blob/develop/cmd/coincapworker/migrate.go



# litecoin节点 

## Login

1. ssh登录主机（47.52.140.177）

```
Michaels-iMac:~ eric$ ssh   root@47.52.140.177  -p  32222  
```

2. 切换到ltc节点 

```
root@wallet-gw:~# sshx  ltc
Welcome to Ubuntu 16.04.2 LTS (GNU/Linux 4.4.0-62-generic x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

Welcome to Alibaba Cloud Elastic Compute Service !

Last login: Tue Mar 13 16:16:26 2018 from 172.31.64.86
root@ltc:~#
```

## 操作

### 进入目录litecore-wallet-service 

```
root@ltc:~/james/litecore-wallet-service# 
``` 

### 启动服务

```
root@ltc:~/james/litecore-wallet-service# ./start.sh 
```


### 停止服务

```
root@ltc:~/james/litecore-wallet-service# ./stop.sh  
```

### 说明  

目录对应代码库为https://gitlab.chainresearch.org/wallet/ltc-wallet-service 

## 配置 

```
root@ltc:~/james/litecore-wallet-service# cat config.js
var config = {
  basePath: '/bws/api',
  disableLogs: false,
  port: 3232, // 服务端口

  // Uncomment to make BWS a forking server
  // cluster: true,

  // Uncomment to set the number or process (will use the nr of availalbe CPUs by default)
  // clusterInstances: 4,

  // https: true,
  // privateKeyFile: 'private.pem',
  // certificateFile: 'cert.pem',
  ////// The following is only for certs which are not
  ////// trusted by nodejs 'https' by default
  ////// CAs like Verisign do not require this
  // CAinter1: '', // ex. 'COMODORSADomainValidationSecureServerCA.crt'
  // CAinter2: '', // ex. 'COMODORSAAddTrustCA.crt'
  // CAroot: '', // ex. 'AddTrustExternalCARoot.crt'
 // mongoDb存储地址，交易等信息
  storageOpts: {
    mongoDb: {
       //uri: 'mongodb://localhost:27017/lws',
      uri: 'mongodb://ltcws:wallet2017@dds-j6cf20f193bab5641.mongodb.rds.aliyuncs.com:3717,dds-j6cf20f193bab5642.mongodb.rds.aliyuncs.com:3717/ltcws?replicaSet=mgset-4808555',
    },
  },
  lockOpts: {
    //  To use locker-server, uncomment this:
    lockerServer: {
      host: 'localhost',
      port: 3231,
    },
  },
  messageBrokerOpts: {
    //  To use message broker server, uncomment this:
    messageBrokerServer: {
      url: 'http://localhost:3380',
    },
  },
  // 区块链浏览器信息
  blockchainExplorerOpts: {
    livenet: {
      provider: 'insight',
      url: 'http://localhost:3001',
    },
    testnet: {
      provider: 'insight',
      //url: 'https://test-insight.bitpay.com:443',
       url: 'http://localhost:3001',
      // Multiple servers (in priority order)
      // url: ['http://a.b.c', 'https://test-insight.bitpay.com:443'],
    },
  },
  pushNotificationsOpts: {
    templatePath: './lib/templates',
    defaultLanguage: 'en',
    defaultUnit: 'btc',
    subjectPrefix: '',
    pushServerUrl: 'https://fcm.googleapis.com/fcm',
    authorizationKey: '',
  },
  fiatRateServiceOpts: {
    defaultProvider: 'BitPay',
    fetchInterval: 60, // in minutes
  },
  // To use email notifications uncomment this:
  // emailOpts: {
  //  host: 'localhost',
  //  port: 25,
  //  ignoreTLS: true,
  //  subjectPrefix: '[Wallet Service]',
  //  from: 'wallet-service@bitcore.io',
  //  templatePath: './lib/templates',
  //  defaultLanguage: 'en',
  //  defaultUnit: 'btc',
  //  publicTxUrlTemplate: {
  //    livenet: 'https://insight.bitpay.com/tx/{{txid}}',
  //    testnet: 'https://test-insight.bitpay.com/tx/{{txid}}',
  //  },
  //},
  //
  // To use sendgrid:
  // var sgTransport = require('nodemail-sendgrid-transport');
  // mailer:sgTransport({
  //  api_user: xxx,
  //  api_key: xxx,
  // });
};
module.exports = config;
```

## 监控 

### 服务进程 

通过pm2自动重启 

### 节点状态  

通过阿里云监控，重点关注硬盘，参考[文档](https://help.aliyun.com/document_detail/25453.html?spm=5176.11065259.1996646101.searchclickresult.6e7a1bcblzQZ1w)
[云服务器 ECS Linux 磁盘空间常见问题处理方法](https://help.aliyun.com/knowledge_detail/52870.html)

##  nginx反向代理配置 

```
ltc.58wallet.io              172.31.64.91:3232
ltc-insight.58wallet.io.     172.31.64.91:3001
```

## 测试

### litecoin区块浏览器 

https://ltc-insight.58wallet.io/insight/  


### API 

https://ltc.58wallet.io/bws/api/v2/feelevels


## 参考  

1. [bitpay](https://github.com/bitpay) 
2. [litecoin-project](https://github.com/litecoin-project/) 









