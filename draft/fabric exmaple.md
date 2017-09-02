
# docker

```
Michaels-iMac:example_cc eric$ docker  ps  
CONTAINER ID        IMAGE                                     COMMAND                  CREATED             STATUS              PORTS                                            NAMES
7585d7ac14f7        dev-peer1.org1.example.com-mycc-v0        "chaincode -peer.a..."   2 hours ago         Up 2 hours                                                           dev-peer1.org1.example.com-mycc-v0
32252323b4e2        dev-peer0.org1.example.com-mycc-v0        "chaincode -peer.a..."   2 hours ago         Up 2 hours                                                           dev-peer0.org1.example.com-mycc-v0
c29d434ff320        hyperledger/fabric-peer:x86_64-1.0.0      "peer node start"        2 hours ago         Up 2 hours          0.0.0.0:7051->7051/tcp, 0.0.0.0:7053->7053/tcp   peer0.org1.example.com
7be3ec658e2b        hyperledger/fabric-peer:x86_64-1.0.0      "peer node start"        2 hours ago         Up 2 hours          0.0.0.0:8056->7051/tcp, 0.0.0.0:8058->7053/tcp   peer1.org2.example.com
de5a93f76ad7        hyperledger/fabric-peer:x86_64-1.0.0      "peer node start"        2 hours ago         Up 2 hours          0.0.0.0:7056->7051/tcp, 0.0.0.0:7058->7053/tcp   peer1.org1.example.com
ce00e8a18fa0        hyperledger/fabric-peer:x86_64-1.0.0      "peer node start"        2 hours ago         Up 2 hours          0.0.0.0:8051->7051/tcp, 0.0.0.0:8053->7053/tcp   peer0.org2.example.com
6f04f50aa37d        hyperledger/fabric-orderer:x86_64-1.0.0   "orderer"                2 hours ago         Up 2 hours          0.0.0.0:7050->7050/tcp                           orderer.example.com
3ba56f289321        hyperledger/fabric-ca:x86_64-1.0.0        "sh -c 'fabric-ca-..."   2 hours ago         Up 2 hours          0.0.0.0:8054->7054/tcp                           ca_peerOrg2
e93d62f612ac        hyperledger/fabric-ca:x86_64-1.0.0        "sh -c 'fabric-ca-..."   2 hours ago         Up 2 hours          0.0.0.0:7054->7054/tcp                           ca_peerOrg1
```

# chaincode节点

```
root@7585d7ac14f7:/# ps  -ef 
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 03:58 ?        00:00:00 chaincode -peer.address=peer1.org1.example.com:7051
root        10     0  0 06:03 pts/0    00:00:00 /bin/bash
root        21    10  0 06:03 pts/0    00:00:00 ps -ef
```

```
root@32252323b4e2:/# ps  -ef 
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 03:58 ?        00:00:00 chaincode -peer.address=peer0.org1.example.com:7051
root         9     0  0 06:20 pts/0    00:00:00 /bin/bash
root        17     9  0 06:20 pts/0    00:00:00 ps -ef
```

# CA  ca_peerOrg1

```
root@800aa93b75b9:/# ps   -ef   
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 06:48 ?        00:00:00 sh -c fabric-ca-server start -b admin:adminpw -d
root         5     1  0 06:48 ?        00:00:00 fabric-ca-server start -b admin:adminpw -d
root         9     0  5 07:24 pts/0    00:00:00 /bin/bash
root        18     9  0 07:24 pts/0    00:00:00 ps -ef
```

fabric-ca-server  是证书服务器？

# docker ca failed 


```
Michaels-iMac:balance-transfer eric$ docker logs   -f    da522a15d1a6  
2017/08/02 04:00:01 [INFO] Created default configuration file at /etc/hyperledger/fabric-ca-server/fabric-ca-server-config.yaml
2017/08/02 04:00:01 [INFO] Starting server in home directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:00:01 [DEBUG] CA Home Directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:00:01 [DEBUG] Making server filenames absolute
2017/08/02 04:00:01 [DEBUG] Initializing default CA in directory /etc/hyperledger/fabric-ca-server
2017/08/02 04:00:01 [DEBUG] Init CA with home /etc/hyperledger/fabric-ca-server and config {CA:{Name: Keyfile:/etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk Certfile:/etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem Chainfile:ca-chain.pem} Signing:0xc4202bde90 CSR:{CN:fabric-ca-server Names:[{C:US ST:North Carolina L: O:Hyperledger OU:Fabric SerialNumber:}] Hosts:[da522a15d1a6 localhost] KeyRequest:<nil> CA:0xc4202c49a0 SerialNumber:} Registry:{MaxEnrollments:-1 Identities:[{Name:admin Pass:adminpw Type:client Affiliation: MaxEnrollments:-1 Attrs:map[hf.Registrar.DelegateRoles:client,user,validator,auditor hf.Revoker:1 hf.IntermediateCA:1 hf.Registrar.Roles:client,user,peer,validator,auditor]}]} Affiliations:map[org1:[department1 department2] org2:[department1]] LDAP:{Enabled:false URL:ldap://<adminDN>:<adminPassword>@<host>:<port>/<base> UserFilter:(uid=%s) GroupFilter:(memberUid=%s) TLS:{Enabled:false CertFiles:[ldap-server-cert.pem] Client:{KeyFile:ldap-client-key.pem CertFile:ldap-client-cert.pem}}} DB:{Type:sqlite3 Datasource:fabric-ca-server.db TLS:{Enabled:false CertFiles:[db-server-cert.pem] Client:{KeyFile:db-client-key.pem CertFile:db-client-cert.pem}}} CSP:0xc4202c56e0 Client:<nil> Intermediate:{ParentServer:{URL: CAName:} TLS:{Enabled:false CertFiles:[] Client:{KeyFile: CertFile:}} Enrollment:{Name: Secret: Profile: Label: CSR:<nil> CAName:}}}
2017/08/02 04:00:01 [DEBUG] CA Home Directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:00:01 [DEBUG] Initializing BCCSP: &{ProviderName:SW SwOpts:0xc4202bb200 Pkcs11Opts:<nil>}
2017/08/02 04:00:01 [DEBUG] Initializing BCCSP with software options &{SecLevel:256 HashFamily:SHA2 Ephemeral:false FileKeystore:0xc4202ce830 DummyKeystore:<nil>}
2017/08/02 04:00:01 [DEBUG] Initialize key material
2017/08/02 04:00:01 [DEBUG] Making CA filenames absolute
2017/08/02 04:00:01 [INFO] The CA key and certificate files already exist
2017/08/02 04:00:01 [INFO] Key file location: /etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk
2017/08/02 04:00:01 [INFO] Certificate file location: /etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem
2017/08/02 04:00:01 [DEBUG] Validating the CA certificate and key
2017/08/02 04:00:01 [DEBUG] Check CA certificate for valid dates
2017/08/02 04:00:01 [DEBUG] Check CA certificate for valid usages
2017/08/02 04:00:01 [DEBUG] Check CA certificate for valid IsCA value
2017/08/02 04:00:01 [DEBUG] Check that key type is supported
2017/08/02 04:00:01 [DEBUG] Check that key size is of appropriate length
2017/08/02 04:00:01 [DEBUG] Check that public key and private key match
Error: Validation of certificate and key failed: Invalid certificate and/or key in files '/etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem' and '/etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk': Public key and private key do not match
2017/08/02 04:45:02 [INFO] Configuration file location: /etc/hyperledger/fabric-ca-server/fabric-ca-server-config.yaml
2017/08/02 04:45:02 [INFO] Starting server in home directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:45:02 [DEBUG] CA Home Directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:45:02 [DEBUG] Making server filenames absolute
2017/08/02 04:45:02 [DEBUG] Initializing default CA in directory /etc/hyperledger/fabric-ca-server
2017/08/02 04:45:02 [DEBUG] Init CA with home /etc/hyperledger/fabric-ca-server and config {CA:{Name: Keyfile:/etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk Certfile:/etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem Chainfile:ca-chain.pem} Signing:0xc42026d0d0 CSR:{CN:fabric-ca-server Names:[{C:US ST:North Carolina L: O:Hyperledger OU:Fabric SerialNumber:}] Hosts:[da522a15d1a6 localhost] KeyRequest:<nil> CA:0xc420270860 SerialNumber:} Registry:{MaxEnrollments:-1 Identities:[{Name:admin Pass:adminpw Type:client Affiliation: MaxEnrollments:-1 Attrs:map[hf.Registrar.Roles:client,user,peer,validator,auditor hf.Registrar.DelegateRoles:client,user,validator,auditor hf.Revoker:1 hf.IntermediateCA:1]}]} Affiliations:map[org2:[department1] org1:[department1 department2]] LDAP:{Enabled:false URL:ldap://<adminDN>:<adminPassword>@<host>:<port>/<base> UserFilter:(uid=%s) GroupFilter:(memberUid=%s) TLS:{Enabled:false CertFiles:[ldap-server-cert.pem] Client:{KeyFile:ldap-client-key.pem CertFile:ldap-client-cert.pem}}} DB:{Type:sqlite3 Datasource:fabric-ca-server.db TLS:{Enabled:false CertFiles:[db-server-cert.pem] Client:{KeyFile:db-client-key.pem CertFile:db-client-cert.pem}}} CSP:0xc420270c20 Client:<nil> Intermediate:{ParentServer:{URL: CAName:} TLS:{Enabled:false CertFiles:[] Client:{KeyFile: CertFile:}} Enrollment:{Name: Secret: Profile: Label: CSR:<nil> CAName:}}}
2017/08/02 04:45:02 [DEBUG] CA Home Directory: /etc/hyperledger/fabric-ca-server
2017/08/02 04:45:02 [DEBUG] Initializing BCCSP: &{ProviderName:SW SwOpts:0xc420259b90 Pkcs11Opts:<nil>}
2017/08/02 04:45:02 [DEBUG] Initializing BCCSP with software options &{SecLevel:256 HashFamily:SHA2 Ephemeral:false FileKeystore:0xc420274ba0 DummyKeystore:<nil>}
2017/08/02 04:45:02 [DEBUG] Initialize key material
2017/08/02 04:45:02 [DEBUG] Making CA filenames absolute
2017/08/02 04:45:02 [INFO] The CA key and certificate files already exist
2017/08/02 04:45:02 [INFO] Key file location: /etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk
2017/08/02 04:45:02 [INFO] Certificate file location: /etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem
2017/08/02 04:45:02 [DEBUG] Validating the CA certificate and key
2017/08/02 04:45:02 [DEBUG] Check CA certificate for valid dates
2017/08/02 04:45:02 [DEBUG] Check CA certificate for valid usages
2017/08/02 04:45:02 [DEBUG] Check CA certificate for valid IsCA value
2017/08/02 04:45:02 [DEBUG] Check that key type is supported
2017/08/02 04:45:02 [DEBUG] Check that key size is of appropriate length
2017/08/02 04:45:02 [DEBUG] Check that public key and private key match
Error: Validation of certificate and key failed: Invalid certificate and/or key in files '/etc/hyperledger/fabric-ca-server-config/ca.org1.example.com-cert.pem' and '/etc/hyperledger/fabric-ca-server-config/0e729224e8b3f31784c8a93c5b8ef6f4c1c91d9e6e577c45c33163609fe40011_sk': Public key and private key do not match
Michaels-iMac:balance-transfer eric$ 
```

## 证书不匹配的问题  

docker镜像是如何生成的？ 
docker镜像peer证书是如何生成的 

是CA的  


peer打包是如何获取ca的？

ca证书或者密钥信息 


