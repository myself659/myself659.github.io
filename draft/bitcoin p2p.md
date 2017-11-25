
# 问题  

litecoin 不能连接到测试节点  

不能同步区块 


Bitcoin has its own custom wire protocol using TCP. 
Peer discovery is by address rumoring, where connected nodes gossip about other potential available peers. 
When a node is new and has nobody to gossip with, 
they make a DNS lookup of specific hostnames which provide a number of known-good peers to make an outgoing connection to. 
If DNS seeds fail and none of the known peers are good, 
Bitcoin Core nodes fall back to a set of hardcoded peers as a last resort.

