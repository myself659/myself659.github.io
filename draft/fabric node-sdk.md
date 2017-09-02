
https://github.com/hyperledger/fabric-samples/  
主要学习这个跑起来就可以了 

https://docs.npmjs.com/getting-started/using-a-package.json 
自动生成package.json文件 

http://javascript.ruanyifeng.com/nodejs/packagejson.html 

安装失败

```
error: [Peer.js]: sendProposal - timed out after:45000
error: [Peer.js]: sendProposal - timed out after:45000
error: [client-utils.js]: sendPeersProposal - Promise is rejected: Error: REQUEST_TIMEOUT
    at Timeout._onTimeout (/Users/eric/go/src/github.com/hyperledger/fabric-samples/balance-transfer/node_modules/fabric-client/lib/Peer.js:100:19)
    at ontimeout (timers.js:386:14)
    at tryOnTimeout (timers.js:250:5)
    at Timer.listOnTimeout (timers.js:214:5)
error: [client-utils.js]: sendPeersProposal - Promise is rejected: Error: REQUEST_TIMEOUT
    at Timeout._onTimeout (/Users/eric/go/src/github.com/hyperledger/fabric-samples/balance-transfer/node_modules/fabric-client/lib/Peer.js:100:19)
    at ontimeout (timers.js:386:14)
    at tryOnTimeout (timers.js:250:5)
    at Timer.listOnTimeout (timers.js:214:5)
[2017-07-25 11:11:03.055] [ERROR] instantiate-chaincode - instantiate proposal was bad
```
chaincode  instantiate  失败


```
error: [client-utils.js]: sendPeersProposal - Promise is rejected: Error: could not find chaincode with name 'mycc' - make sure the chaincode mycc has been successfully instantiated and try again
    at /Users/eric/go/src/github.com/hyperledger/fabric-samples/balance-transfer/node_modules/grpc/src/node/src/client.js:434:17
error: [client-utils.js]: sendPeersProposal - Promise is rejected: Error: could not find chaincode with name 'mycc' - make sure the chaincode mycc has been successfully instantiated and try again
    at /Users/eric/go/src/github.com/hyperledger/fabric-samples/balance-transfer/node_modules/grpc/src/node/src/client.js:434:17
[2017-07-25 11:58:31.967] [ERROR] invoke-chaincode - transaction proposal was bad
[2017-07-25 11:58:31.967] [ERROR] invoke-chaincode - transaction proposal was bad
[2017-07-25 11:58:31.967] [ERROR] invoke-chaincode - Failed to send Proposal or receive valid response. Response null or status is not 200. exiting...
[2017-07-25 11:58:31.967] [ERROR] invoke-chaincode - Failed to order the transaction. Error code: undefined
```


```
[2017-07-26 14:29:06.414] [DEBUG] Helper - [FileKeyValueStore.js]: FileKeyValueStore -- setValue
info: [packager/Golang.js]: packaging GOLANG from github.com/example_cc
events.js:160
      throw er; // Unhandled 'error' event
      ^

Error: ENOENT: no such file or directory, lstat '/Users/eric/go/src/github.com/hyperledger/fabric-samples/storechain/app/artifacts/src/github.com/example_cc'
```