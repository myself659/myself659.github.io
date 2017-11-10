# docker compose  配置文件详解 


http://www.jianshu.com/p/2217cfed29d7 



# docker-compose  failed 


```
Michaels-iMac:prometheus eric$ docker-compose    -f    docker-compose.yml      up     
Creating network "prometheus_default" with the default driver
Pulling metrics (prom/prometheus:latest)...
ERROR: Get https://registry-1.docker.io/v2/: proxyconnect tcp: dial tcp 192.168.65.1:1087: getsockopt: connection refused
```

https://linuxconfig.org/docker-dial-tcp-lookup-index-docker-io-no-such-host-fix 


http://blog.csdn.net/halcyonbaby/article/details/39475765 


错误的代理配置