

#   调用点 



```

func ipnsPubFunc(ipfs *core.IpfsNode, k ci.PrivKey) mfs.PubFunc {
	return func(ctx context.Context, c *cid.Cid) error {
		return ipfs.Namesys.Publish(ctx, k, path.FromCid(c))
	}
}

```


https://github.com/ipfs/go-ipfs/blob/master/docs/fuse.md 



# ipfs  publish 


```
func publish(ctx context.Context, n *core.IpfsNode, k crypto.PrivKey, ref path.Path, opts *publishOpts) (*IpnsEntry, error) {

	if opts.verifyExists {
		// verify the path exists
		_, err := core.Resolve(ctx, n.Namesys, n.Resolver, ref)
		if err != nil {
			return nil, err
		}
	}

	eol := time.Now().Add(opts.pubValidTime)
	err := n.Namesys.PublishWithEOL(ctx, k, ref, eol)
	if err != nil {
		return nil, err
	}

	pid, err := peer.IDFromPrivateKey(k)
	if err != nil {
		return nil, err
	}

	return &IpnsEntry{
		Name:  pid.Pretty(),
		Value: ref.String(),
	}, nil
}
```

https://ipfs.io/ipfs/QmdPtC3T7Kcu9iJg6hYzLBWR5XCDcYMY7HV685E3kH3EcS/2015/09/15/hosting-a-website-on-ipfs/ 


# CID PATH 


# 认识 

不变地址 vs 改变内容  

不可控制性 

https://github.com/ipfs/faq/issues/16 


IPNS is there as a way to have an unchanging link to changing data. in IPFS the link is the hash of the data, so you can never change the data without changing the link.

This means IPFS cannot support dynamic content (by itself), IPNS is a way to have an unchanging link refer to changing content. That's its goal. 


If the owner of that IPNS address dies or goes away, will the swarm be capable of giving you the last seen resolution to that address? Or will it be a broken link?

Should IPNS be considered when it comes to semi-realtime communication? Or should it just be used to bootstrap something else? 

让拥有者有很好的控制能力呢 

如何快速更新路由  减少旧内容的非法展示内容？

# DNS
Alternatively, the good ol’ DNS can be used to resolve IPFS names. The dnslink= TXT record should point to the IPFS path.
For example, here’s the TXT record of the IPFS website:
ipfs.io. 11 IN TXT "dnslink=/ipfs/QmTzQ1JRkWErjk39mryYw2WVaphAZNAREyMchXzYQ7c15n"
Because of that TXT record, the website can be accessed by the /ipns/ipfs.io path, instead of an unfriendly peer ID.

如何操作呢？

TXT 如何添加 添加到什么文件中？


https://github.com/ipfs-shipyard/shared-editing-demo




