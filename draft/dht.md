

# dht 

DHT全称叫分布式哈希表(Distributed Hash Table)，是一种分布式存储方法。在不需要服务器的情况下，每个客户端负责一个小范围的路由，并负责存储一小部分数据，从而实现整个DHT网络的寻址和存储。新版BitComet允许同行连接DHT网络和Tracker，也就是说在完全不连上Tracker服务器的情况下，也可以很好的下载，因为它可以在DHT网络中寻找下载同一文件的其他用户

# 原理 

## 对什么内容进行hash计算 


## 基本定义  

node 
nodeid 

##  查找  


## 插入 

## 更新 

## 冲突处理 

## 路由表 



6.824 2015 Lecture 22: P2P, DHTs, and Chord

Lecture outline:
  peer-to-peer (P2P)
  BitTorrent
  DHTs
  Chord

Peer-to-peer
P2P定义
  [user computers, files, direct xfers]
  users computers talk directly to each other to implement service
    in contrast to user computers talking to central servers
  could be closed or open
  examples:
    skype, video and music players, file sharing

Why might P2P be a win?
  spreads network/caching costs over users
  absence of server may mean:
    easier to deploy
    less chance of overload
    single failure won't wreck the whole system
    harder to attack

Why don't all Internet services use P2P?
  can be hard to find data items over millions of users
  user computers not as reliable than managed servers
  if open, can be attacked via evil participants

The result is that P2P has some successful niches:
  Client-client video/music, where serving costs are high
  Chat (user to user anyway; privacy and control)
  Popular data but owning organization has no money
  No natural single owner or controller (Bitcoin)
  Illegal file sharing

Example: classic BitTorrent
  a cooperative download system, very popular!
  user clicks on download link for e.g. latest Linux kernel distribution
    gets torrent file w/ content hash and IP address of tracker
  user's BT client talks to tracker
    tracker tells it list of other user clients w/ downloaded file
  user't BT client talks to one or more client's w/ the file
  user's BT client tells tracker it has a copy now too
  user's BT client serves the file to others for a while
  the point:
    provides huge download b/w w/o expensive server/link

BitTorrent can also use a DHT instead of / as well as a tracker
BitTorrent使用DHT代替tracker服务器
  this is the topic of today's readings
  BT clients cooperatively implement a giant key/value store
  "distributed hash table"
  the key is the file content hash ("infohash")
  the value is the IP address of a client willing to serve the file
    Kademlia can store multiple values for a key
  client does get(infohash) to find other clients willing to serve
    and put(infohash, self) to register itself as willing to serve
  client also joins the DHT to help implement it

Why might the DHT be a win for BitTorrent?
  single giant tracker, less fragmented than many trackers
    so clients more likely to find each other
  maybe a classic tracker too exposed to legal &c attacks
  it's not clear that BitTorrent depends heavily on the DHT
    mostly a backup for classic trackers?

How do DHTs work?
DHT是如何工作的？
可扩展DHT查找：
Scalable DHT lookup:
  Key/value store spread over millions of nodes
  Typical DHT interface:
    put(key, value)
    get(key) -> value
DHT接口：

  loose consistency; likely that get(k) sees put(k), but no guarantee
  松一致性
  loose guarantees about keeping data alive


Why is it hard?
为什么难以实现 
  Millions of participating nodes
  几百万个分散的节点
  Could broadcast/flood request -- but too many messages
  如果能够广播请求就会导致大量的请求
  每个节点都需要知道其他的节点，如果采用hash很容易，但是保证几百万个节点更新就很困难了
  Every node could know about every other node
    Then hashing is easy
    But keeping a million-node table up to date is hard
    我们需要较小的状态，较少的消息和查找
  We want modest state, and modest number of messages/lookup

Basic idea 基本想法 

  Impose a data structure (e.g. tree) over the nodes
    Each node has references to only a few other nodes
  Lookups traverse the data structure -- "routing"
    I.e. hop from node to node
  DHT should route get() to same node as previous put()

Example: The "Chord" peer-to-peer lookup system
  By Stoica, Morris, Karger, Kaashoek and Balakrishnan; 2001

Chord's ID-space topology
  Ring: All IDs are 160-bit numbers, viewed in a ring.
  Each node has an ID, randomly chosen 
  每个节点有一个ID，随机选择 冲突如何解决 网络分化会怎么办 我是a， 你是a， 我不知道你是a， 你也不知道我是a 

Assignment of key IDs to node IDs?
keyID 是如何分配到node ID?
  Key stored on first node whose ID is equal to or greater than key ID.
    Closeness is defined as the "clockwise distance"
  If node and key IDs are uniform, we get reasonable load balance.
  So keys IDs should be hashes (e.g. bittorrent infohash)

Basic routing -- correct but slow
  Query is at some node.
  Node needs to forward the query to a node "closer" to key.
    If we keep moving query closer, eventually we'll win.
  Each node knows its "successor" on the ring.
  k表示什么 key？
  n表示当前节点id
    n.lookup(k):
      if n < k <= n.successor
        return n.successor
      else
        forward to n.successor
  I.e. forward query in a clockwise direction until done
  n.successor must be correct!
    otherwise we may skip over the responsible node
    and get(k) won't see data inserted by put(k)

Forwarding through successor is slow
  Data structure is a linked list: O(n)
  Can we make it more like a binary search?
    Need to be able to halve the distance at each step.

i表示有多少位  2**i  如160位系统

log(n) "finger table" routing:
  Keep track of nodes exponentially further away:
    New state: f[i] contains successor of n + 2^i
    finger table 存储 对应的继承者节点
    finger如何更新？
    n.lookup(k):
      if n < k <= n.successor:
        return successor
      else:
        n' = closest_preceding_node(k) -- in f[]
        forward to n'

for a six-bit system, maybe node 8's looks like this:
  0: 14
  1: 14
  2: 14
  3: 21
  4: 32
  5: 42
  
  这样的：
  0:  2**0 14
  1:  2**1 14
  2:  2**2 14
  3:  2**3 21
  4:  2**3 32
  5:  2**4 42

Why do lookups now take log(n) hops?
  One of the fingers must take you roughly half-way to target

There's a binary lookup tree rooted at every node
  Threaded through other nodes' finger tables
  This is *better* than simply arranging the nodes in a single tree
    Every node acts as a root, so there's no root hotspot
    But a lot more state in total

Is log(n) fast or slow?
  For a million nodes it's 20 hops.
  If each hop takes 50 ms, lookups take a second.
  If each hop has 10% chance of failure, it's a couple of timeouts.
  So in practice log(n) is better than O(n) but not great.

一个新节点是如何获取新正确的路由表的？
How does a new node acquire correct tables?
  General approach:
    Assume system starts out w/ correct routing tables.
    Use routing tables to help the new node find information.
    Add new node in a way that maintains correctness.
    新建节点m:
  New node m:
    Sends a lookup for its own key, to any existing node.
      This yields m.successor
    发送一个查找请求到所有存在的节点，这个请求带了自身的key，这样就知道了节点m的继承节点
    m asks its successor for its entire finger table.
    m从他的继承节点获取他所有的finger table

  At this point the new node can forward queries correctly
  Tweaks its own finger table in background
    By looking up each m + 2^i

新节点引入，如何更新前向节点与后向节点？
Does routing *to* new node m now work?
  If m doesn't do anything,
    lookup will go to where it would have gone before m joined.
    I.e. to m's predecessor.
    Which will return its n.successor -- which is not m.
  So, for correctness, m's predecessor needs to set successor to m.
    Each node keeps track of its current predecessor.
    When m joins, tells its successor that its predecessor has changed.
    Periodically ask your successor who its predecessor is:
      If that node is closer to you, switch to that guy.
    So if we have x m y
      x.successor will be y (now incorrect)
      y.predecessor will be m
      x will ask its x.successor for predecessor
        x learns about m
        sets x.successor to m
        tells m "x is your predecessor"
        called "stabilization"
  Correct successors are sufficient for correct lookups!

并发加入
What about concurrent joins?
  Two new nodes with very close ids, might have same successor.
  Example:
    Initially 40 then 70
    50 and 60 join concurrently
    at first 40, 50, and 60 think their successor is 70!
    which means lookups for e.g. 45 will yield 70, not 50
    after one stabilization, 40 and 50 will learn about 60
    then 40 will learn about 50

由于新节点加入，每个节点定时需要维护自己fingler，通过查找
To maintain log(n) lookups as nodes join,
  Every one periodically looks up each finger (each n + 2^i)

Chord's routing is conceptually similar to Kademlia's
  Finger table similar to bucket levels
    Both halve the metric distance for each step
    Both are about speed and can be imprecise
  n.successor similar to Kademlia's requirement that
    each node know of all the nodes that are very close in xor-space
    in both cases care is needed to ensure that different lookups
      for same key converge on exactly the same node

如果节点失效，会怎么办？
What about node failures?
  Assume nodes fail w/o warning. Strictly harder than graceful departure.
  Two issues:
    Other nodes' routing tables refer to dead node.
    Dead node's predecessor has no successor.
  If you try to route via dead node, detect timeout, treat as empty table entry.
    I.e. route to numerically closer entry instead.
  For dead successor
    Failed node might have been just before key ID!
      So we need to know what its n.successor was
    Maintain a _list_ of successors: r successors.
    Lookup answer is first live successor >= key
      or forward to *any* successor < key

Kademlia has a faster plan for this
  send alpha (or k) lookup RPCs in parallel, to different nodes
  send more lookups as previous ones return info about nodes closer to key
  single non-responsive node won't cause lookup to suffer a timeout

处理不可到达的节点在路由期间是非常重要的
Dealing with unreachable nodes during routing is extremely important
  "Churn" is very high in open p2p networks
  在一个开放P2P网络中搅动非常多
  People close their laptops, move WiFi APs, &c pretty often
  Measurement of Bittorrent/Kademlia suggest lookups are not very fast

Geographical/network locality -- reducing lookup time
  Lookup takes log(n) messages.
    But they are to random nodes on the Internet!
    Will often be very far away.
  Can we route through nodes close to us on underlying network?
  This boils down to whether we have choices:
    If multiple correct next hops, we can try to choose closest.

Idea:
  to fill a finger table entry, collect multiple nodes near n+2^i on ring
  perhaps by asking successor to n+2^i for its r successors
  use lowest-ping one as i'th finger table entry

What's the effect?
  Individual hops are lower latency.
  But less and less choice (lower node density) as you get close in ID space.
  So last few hops likely to be very long. 
  Though if you are reading, and any replica will do,
    you still have choice even at the end.

What about security?
  Self-authenticating data, e.g. key = SHA1(value)
    So DHT node can't forge data
    DHT节点不能伪造数据  当然是不可更改数据
    Of course it's annoying to have immutable data...
  Can someone cause millions of made-up hosts to join?
    They don't exist, so routing will break?
    Don't believe new node unless it responds to ping, w/ random token.
  Can a DHT node claim that data doesn't exist?
    Yes, though perhaps you can check other replicas
  Can a host join w/ IDs chosen to sit under every replica?
    Or "join" many times, so it is most of the DHT nodes?
    Maybe you can require (and check) that node ID = SHA1(IP address)

为什么不维护一张完整的路由表， 这样就可以一跳路由？
Why not just keep complete routing tables?
  So you can always route in one hop?
  Danger in large systems: timeouts or cost of keeping tables up to date.
  这在大规模系统是很危险的 超时 维护路由表成本 实时更新

How to manage data?
怎么管理数据
  Here is the most popular plan.
  DHT并不保证数据持久化存储
  DHT doesn't guarantee durable storage
    So whoever inserted must re-insert periodically if they care
    May want to automatically expire if data goes stale (bittorrent)
    数据会自动老化（过期）如果数据不活跃
    DHT节点会拷贝每一个key/value项
  DHT does replicate each key/value item
    On the nodes with IDs closest to the key, where looks will find them
    Replication can help spread lookup load as well as tolerate faults
  当一个节点加入的时候，继承节点会将一些key移动到新加入的节点
  When a node joins:
    successor moves some keys to it
  当一个节点失效的时候，继承节点可能已经有备份
  When a node fails:
    successor probably already has a replica
    but r'th successor now needs a copy

Retrospective
  DHTs seem very promising for finding data in large p2p systems
    Decentralization seems good for load, fault tolerance
  But: the security problems are difficult
  But: churn is a serious problem, particularly if log(n) is big
  So DHTs have not had the impact that many hoped for


DHT设计目标：

1. 灵活地建立keys与物理节点的映射关系  
2. 控制较小网络范围 
3. 较小程度 fanout 
4. 基于本地路由计算路径 
5. 对节点变化有较强的robustness 
6. 弹性路由  
7. locality   

存储策略：

1. 通过soft state 实现最好的缓存管理与维护  
2. 通过多写实现容错


实例分析：

http://www.bittorrent.org/beps/bep_0005.html

有哪些例子 

需要解决哪些问题？

