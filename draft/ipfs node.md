
# node  

位置:

/Users/eric/go/src/github.com/ipfs/go-ipfs/core/core.go

```
// IpfsNode is IPFS Core module. It represents an IPFS instance.
type IpfsNode struct {

	// Self
	Identity peer.ID // the local node's identity

	Repo repo.Repo

	// Local node
	Pinning        pin.Pinner // the pinning manager
	Mounts         Mounts     // current mount state, if any.
	PrivateKey     ic.PrivKey // the local node's private Key
	PNetFingerpint []byte     // fingerprint of private network

	// Services
	Peerstore  pstore.Peerstore     // storage for other Peer instances
	Blockstore bstore.GCBlockstore  // the block store (lower level)
	Filestore  *filestore.Filestore // the filestore blockstore
	BaseBlocks bstore.Blockstore    // the raw blockstore, no filestore wrapping
	GCLocker   bstore.GCLocker      // the locker used to protect the blockstore during gc
	Blocks     bserv.BlockService   // the block service, get/add blocks.
	DAG        merkledag.DAGService // the merkle dag service, get/add objects.
	Resolver   *path.Resolver       // the path resolution system
	Reporter   metrics.Reporter
	Discovery  discovery.Service
	FilesRoot  *mfs.Root

	// Online
	PeerHost     p2phost.Host        // the network host (server+client)
	Bootstrapper io.Closer           // the periodic bootstrapper
	Routing      routing.IpfsRouting // the routing system. recommend ipfs-dht
	Exchange     exchange.Interface  // the block exchange + strategy (bitswap)
	Namesys      namesys.NameSystem  // the name system, resolves paths to hashes
	Ping         *ping.PingService
	Reprovider   *rp.Reprovider // the value reprovider system
	IpnsRepub    *ipnsrp.Republisher

	Floodsub *floodsub.PubSub
	P2P      *p2p.P2P

	proc goprocess.Process
	ctx  context.Context

	mode         mode
	localModeSet bool
}

```