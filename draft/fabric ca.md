
```
Michaels-iMac:storechain eric$ tree   artifacts/ 
artifacts/
├── base.yaml
├── channel
│   ├── configtx.yaml
│   ├── crypto-config
│   │   ├── ordererOrganizations
│   │   │   └── example.com
│   │   │       ├── ca
│   │   │       │   ├── 0d46ccf0e9436c1bc3b6e2bf80cdb202c4943604f95c72ee0ff839d3ec300719_sk
│   │   │       │   └── ca.example.com-cert.pem
│   │   │       ├── msp
│   │   │       │   ├── admincerts
│   │   │       │   │   └── Admin@example.com-cert.pem
│   │   │       │   ├── cacerts
│   │   │       │   │   └── ca.example.com-cert.pem
│   │   │       │   └── tlscacerts
│   │   │       │       └── tlsca.example.com-cert.pem
│   │   │       ├── orderers
│   │   │       │   └── orderer.example.com
│   │   │       │       ├── msp
│   │   │       │       │   ├── admincerts
│   │   │       │       │   │   └── Admin@example.com-cert.pem
│   │   │       │       │   ├── cacerts
│   │   │       │       │   │   └── ca.example.com-cert.pem
│   │   │       │       │   ├── keystore
│   │   │       │       │   │   └── 2fb065725bf1b7e2811c0e8ca8d37f5a951fc4cd1162a47aad8accf9ddd10291_sk
│   │   │       │       │   ├── signcerts
│   │   │       │       │   │   └── orderer.example.com-cert.pem
│   │   │       │       │   └── tlscacerts
│   │   │       │       │       └── tlsca.example.com-cert.pem
│   │   │       │       └── tls
│   │   │       │           ├── ca.crt
│   │   │       │           ├── server.crt
│   │   │       │           └── server.key
│   │   │       ├── tlsca
│   │   │       │   ├── 6a211ed18880b4db3867831c977809902713b8e321a5ab55ecc104dafc2eec49_sk
│   │   │       │   └── tlsca.example.com-cert.pem
│   │   │       └── users
│   │   │           └── Admin@example.com
│   │   │               ├── msp
│   │   │               │   ├── admincerts
│   │   │               │   │   └── Admin@example.com-cert.pem
│   │   │               │   ├── cacerts
│   │   │               │   │   └── ca.example.com-cert.pem
│   │   │               │   ├── keystore
│   │   │               │   │   └── db670eed8487a93c35ae448b9f84c2f241a7a8c87df0544fc1dc08baf7832aa0_sk
│   │   │               │   ├── signcerts
│   │   │               │   │   └── Admin@example.com-cert.pem
│   │   │               │   └── tlscacerts
│   │   │               │       └── tlsca.example.com-cert.pem
│   │   │               └── tls
│   │   │                   ├── ca.crt
│   │   │                   ├── server.crt
│   │   │                   └── server.key
│   │   └── peerOrganizations
│   │       ├── org1.example.com
│   │       │   ├── ca
│   │       │   │   ├── 0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk
│   │       │   │   └── ca.org1.example.com-cert.pem
│   │       │   ├── msp
│   │       │   │   ├── admincerts
│   │       │   │   │   └── Admin@org1.example.com-cert.pem
│   │       │   │   ├── cacerts
│   │       │   │   │   └── ca.org1.example.com-cert.pem
│   │       │   │   └── tlscacerts
│   │       │   │       └── tlsca.org1.example.com-cert.pem
│   │       │   ├── peers
│   │       │   │   ├── peer0.org1.example.com
│   │       │   │   │   ├── msp
│   │       │   │   │   │   ├── admincerts
│   │       │   │   │   │   │   └── Admin@org1.example.com-cert.pem
│   │       │   │   │   │   ├── cacerts
│   │       │   │   │   │   │   └── ca.org1.example.com-cert.pem
│   │       │   │   │   │   ├── keystore
│   │       │   │   │   │   │   └── 27db82c96b1482480baa1c75f80e5cce249beaab27b70c741bb0e2554355957e_sk
│   │       │   │   │   │   ├── signcerts
│   │       │   │   │   │   │   └── peer0.org1.example.com-cert.pem
│   │       │   │   │   │   └── tlscacerts
│   │       │   │   │   │       └── tlsca.org1.example.com-cert.pem
│   │       │   │   │   └── tls
│   │       │   │   │       ├── ca.crt
│   │       │   │   │       ├── server.crt
│   │       │   │   │       └── server.key
│   │       │   │   └── peer1.org1.example.com
│   │       │   │       ├── msp
│   │       │   │       │   ├── admincerts
│   │       │   │       │   │   └── Admin@org1.example.com-cert.pem
│   │       │   │       │   ├── cacerts
│   │       │   │       │   │   └── ca.org1.example.com-cert.pem
│   │       │   │       │   ├── keystore
│   │       │   │       │   │   └── fdee12a3510fde3155c37128cfec26090ae249bfbca28f884e60c21338493edd_sk
│   │       │   │       │   ├── signcerts
│   │       │   │       │   │   └── peer1.org1.example.com-cert.pem
│   │       │   │       │   └── tlscacerts
│   │       │   │       │       └── tlsca.org1.example.com-cert.pem
│   │       │   │       └── tls
│   │       │   │           ├── ca.crt
│   │       │   │           ├── server.crt
│   │       │   │           └── server.key
│   │       │   ├── tlsca
│   │       │   │   ├── 945092d936f5838c5a6f6484db974d857933706737d00d04bf65f74e3976f9f8_sk
│   │       │   │   └── tlsca.org1.example.com-cert.pem
│   │       │   └── users
│   │       │       ├── Admin@org1.example.com
│   │       │       │   ├── msp
│   │       │       │   │   ├── admincerts
│   │       │       │   │   │   └── Admin@org1.example.com-cert.pem
│   │       │       │   │   ├── cacerts
│   │       │       │   │   │   └── ca.org1.example.com-cert.pem
│   │       │       │   │   ├── keystore
│   │       │       │   │   │   └── 5890f0061619c06fb29dea8cb304edecc020fe63f41a6db109f1e227cc1cb2a8_sk
│   │       │       │   │   ├── signcerts
│   │       │       │   │   │   └── Admin@org1.example.com-cert.pem
│   │       │       │   │   └── tlscacerts
│   │       │       │   │       └── tlsca.org1.example.com-cert.pem
│   │       │       │   └── tls
│   │       │       │       ├── ca.crt
│   │       │       │       ├── server.crt
│   │       │       │       └── server.key
│   │       │       └── User1@org1.example.com
│   │       │           ├── msp
│   │       │           │   ├── admincerts
│   │       │           │   │   └── User1@org1.example.com-cert.pem
│   │       │           │   ├── cacerts
│   │       │           │   │   └── ca.org1.example.com-cert.pem
│   │       │           │   ├── keystore
│   │       │           │   │   └── 73cdc0072c7203f1ec512232c780fc84acc9752ef30ebc16be1f4666c02b614b_sk
│   │       │           │   ├── signcerts
│   │       │           │   │   └── User1@org1.example.com-cert.pem
│   │       │           │   └── tlscacerts
│   │       │           │       └── tlsca.org1.example.com-cert.pem
│   │       │           └── tls
│   │       │               ├── ca.crt
│   │       │               ├── server.crt
│   │       │               └── server.key
│   │       └── org2.example.com
│   │           ├── ca
│   │           │   ├── a7d47efa46a6ba07730c850fed2c1375df27360d7227f48cdc2f80e505678005_sk
│   │           │   └── ca.org2.example.com-cert.pem
│   │           ├── msp
│   │           │   ├── admincerts
│   │           │   │   └── Admin@org2.example.com-cert.pem
│   │           │   ├── cacerts
│   │           │   │   └── ca.org2.example.com-cert.pem
│   │           │   └── tlscacerts
│   │           │       └── tlsca.org2.example.com-cert.pem
│   │           ├── peers
│   │           │   ├── peer0.org2.example.com
│   │           │   │   ├── msp
│   │           │   │   │   ├── admincerts
│   │           │   │   │   │   └── Admin@org2.example.com-cert.pem
│   │           │   │   │   ├── cacerts
│   │           │   │   │   │   └── ca.org2.example.com-cert.pem
│   │           │   │   │   ├── keystore
│   │           │   │   │   │   └── 0d9f72608133ee627b570b6af6877666bc8f365746f9329d6dd8a5f54e53e2ab_sk
│   │           │   │   │   ├── signcerts
│   │           │   │   │   │   └── peer0.org2.example.com-cert.pem
│   │           │   │   │   └── tlscacerts
│   │           │   │   │       └── tlsca.org2.example.com-cert.pem
│   │           │   │   └── tls
│   │           │   │       ├── ca.crt
│   │           │   │       ├── server.crt
│   │           │   │       └── server.key
│   │           │   └── peer1.org2.example.com
│   │           │       ├── msp
│   │           │       │   ├── admincerts
│   │           │       │   │   └── Admin@org2.example.com-cert.pem
│   │           │       │   ├── cacerts
│   │           │       │   │   └── ca.org2.example.com-cert.pem
│   │           │       │   ├── keystore
│   │           │       │   │   └── 27ccb54a06020260c66c65bec91f91e1c9043e3076d3d6128692e7271c4c7a2c_sk
│   │           │       │   ├── signcerts
│   │           │       │   │   └── peer1.org2.example.com-cert.pem
│   │           │       │   └── tlscacerts
│   │           │       │       └── tlsca.org2.example.com-cert.pem
│   │           │       └── tls
│   │           │           ├── ca.crt
│   │           │           ├── server.crt
│   │           │           └── server.key
│   │           ├── tlsca
│   │           │   ├── 7bb8ba3ff11d3c8cf592bd4326062e77d06ac4963c7b7ae459284dfbd3eb5aac_sk
│   │           │   └── tlsca.org2.example.com-cert.pem
│   │           └── users
│   │               ├── Admin@org2.example.com
│   │               │   ├── msp
│   │               │   │   ├── admincerts
│   │               │   │   │   └── Admin@org2.example.com-cert.pem
│   │               │   │   ├── cacerts
│   │               │   │   │   └── ca.org2.example.com-cert.pem
│   │               │   │   ├── keystore
│   │               │   │   │   └── 1995b11d6573ed3be52fcd7a5fa477bc0f183e1f5f398c8281d0ce7c2c75a076_sk
│   │               │   │   ├── signcerts
│   │               │   │   │   └── Admin@org2.example.com-cert.pem
│   │               │   │   └── tlscacerts
│   │               │   │       └── tlsca.org2.example.com-cert.pem
│   │               │   └── tls
│   │               │       ├── ca.crt
│   │               │       ├── server.crt
│   │               │       └── server.key
│   │               └── User1@org2.example.com
│   │                   ├── msp
│   │                   │   ├── admincerts
│   │                   │   │   └── User1@org2.example.com-cert.pem
│   │                   │   ├── cacerts
│   │                   │   │   └── ca.org2.example.com-cert.pem
│   │                   │   ├── keystore
│   │                   │   │   └── 585175c83bac91fc0c1ce8f9d0ff9aefa47c565678f100ca8673db249ee785ac_sk
│   │                   │   ├── signcerts
│   │                   │   │   └── User1@org2.example.com-cert.pem
│   │                   │   └── tlscacerts
│   │                   │       └── tlsca.org2.example.com-cert.pem
│   │                   └── tls
│   │                       ├── ca.crt
│   │                       ├── server.crt
│   │                       └── server.key
│   ├── cryptogen.yaml
│   ├── genesis.block
│   └── mychannel.tx
├── docker-compose.yaml
└── src
    └── github.com
        ├── example_cc
        │   └── example_cc.go
        └── storecc
            └── main.go

115 directories, 109 files
Michaels-iMac:storechain eric$ 
```

用户私钥地址：
```
Michaels-iMac:User1@org1.example.com eric$ cat msp/keystore/73cdc0072c7203f1ec512232c780fc84acc9752ef30ebc16be1f4666c02b614b_sk   
-----BEGIN PRIVATE KEY-----
MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgaYlbFIz6yVz0SYqh
nrhdTCb797PBwSwtCw9HtOkbqQGhRANCAAR1el/5FrkCusYV3vTnp0GSgyO/82se
2I2JUb1JR4AqKiiMvHxQGrd4JRm1V9aKNqGe3DCVZRZ27oj3a4OL5Mo3
-----END PRIVATE KEY-----
Michaels-iMac:User1@org1.example.com eric$ pwd
/Users/eric/go/src/github.com/hyperledger/fabric-samples/storechain/artifacts/channel/crypto-config/peerOrganizations/org1.example.com/users/User1@org1.example.com

```