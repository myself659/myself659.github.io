
## server启动代码  


/Users/eric/go/src/github.com/ethereum/go-ethereum/node/api.go

```
func (api *PrivateAdminAPI) StartRPC(host *string, port *int, cors *string, apis *string) (bool, error) {
	api.node.lock.Lock()
	defer api.node.lock.Unlock()

	if api.node.httpHandler != nil {
		return false, fmt.Errorf("HTTP RPC already running on %s", api.node.httpEndpoint)
	}

	if host == nil {
		h := DefaultHTTPHost
		if api.node.config.HTTPHost != "" {
			h = api.node.config.HTTPHost
		}
		host = &h
	}
	if port == nil {
		port = &api.node.config.HTTPPort
	}

	allowedOrigins := api.node.config.HTTPCors
	if cors != nil {
		allowedOrigins = nil
		for _, origin := range strings.Split(*cors, ",") {
			allowedOrigins = append(allowedOrigins, strings.TrimSpace(origin))
		}
	}

	modules := api.node.httpWhitelist
	if apis != nil {
		modules = nil
		for _, m := range strings.Split(*apis, ",") {
			modules = append(modules, strings.TrimSpace(m))
		}
	}

	if err := api.node.startHTTP(fmt.Sprintf("%s:%d", *host, *port), api.node.rpcAPIs, modules, allowedOrigins); err != nil {
		return false, err
	}
	return true, nil
}
```

api来源服务 

```
// Start create a live P2P node and starts running it.
func (n *Node) Start() error {
	n.lock.Lock()
	defer n.lock.Unlock()

	// Short circuit if the node's already running
	if n.server != nil {
		return ErrNodeRunning
	}
	if err := n.openDataDir(); err != nil {
		return err
	}

	// Initialize the p2p server. This creates the node key and
	// discovery databases.
	n.serverConfig = n.config.P2P
	n.serverConfig.PrivateKey = n.config.NodeKey()
	n.serverConfig.Name = n.config.NodeName()
	if n.serverConfig.StaticNodes == nil {
		n.serverConfig.StaticNodes = n.config.StaticNodes()
	}
	if n.serverConfig.TrustedNodes == nil {
		n.serverConfig.TrustedNodes = n.config.TrustedNodes()
	}
	if n.serverConfig.NodeDatabase == "" {
		n.serverConfig.NodeDatabase = n.config.NodeDB()
	}
	running := &p2p.Server{Config: n.serverConfig}
	log.Info("Starting peer-to-peer node", "instance", n.serverConfig.Name)

	// Otherwise copy and specialize the P2P configuration
	services := make(map[reflect.Type]Service)
	for _, constructor := range n.serviceFuncs {
		// Create a new context for the particular service
		ctx := &ServiceContext{
			config:         n.config,
			services:       make(map[reflect.Type]Service),
			EventMux:       n.eventmux,
			AccountManager: n.accman,
		}
		for kind, s := range services { // copy needed for threaded access
			ctx.services[kind] = s
		}
		// Construct and save the service
		service, err := constructor(ctx)
		if err != nil {
			return err
		}
		kind := reflect.TypeOf(service)
		if _, exists := services[kind]; exists {
			return &DuplicateServiceError{Kind: kind}
		}
		services[kind] = service
	}
	// Gather the protocols and start the freshly assembled P2P server
	for _, service := range services {
		running.Protocols = append(running.Protocols, service.Protocols()...)
	}
	if err := running.Start(); err != nil {
		if errno, ok := err.(syscall.Errno); ok && datadirInUseErrnos[uint(errno)] {
			return ErrDatadirUsed
		}
		return err
	}
	// Start each of the services
	started := []reflect.Type{}
	for kind, service := range services {
		// Start the next service, stopping all previous upon failure
		if err := service.Start(running); err != nil {
			for _, kind := range started {
				services[kind].Stop()
			}
			running.Stop()

			return err
		}
		// Mark the service started for potential cleanup
		started = append(started, kind)
	}
	// Lastly start the configured RPC interfaces
	if err := n.startRPC(services); err != nil {
		for _, service := range services {
			service.Stop()
		}
		running.Stop()
		return err
	}
	// Finish initializing the startup
	n.services = services
	n.server = running
	n.stop = make(chan struct{})

	return nil
}
```

/Users/eric/go/src/github.com/myself659/go-ethereum/node/service.go 
服务接口 
对应有哪些实例了？


```
func defaultNodeConfig() node.Config {
	cfg := node.DefaultConfig
	cfg.Name = clientIdentifier
	cfg.Version = params.VersionWithCommit(gitCommit)
	cfg.HTTPModules = append(cfg.HTTPModules, "eth", "shh")
	cfg.WSModules = append(cfg.WSModules, "eth", "shh")
	cfg.IPCPath = "geth.ipc"
	return cfg
}
```
连接处理

```
// Serve a new connection.
func (c *conn) serve(ctx context.Context) {
	c.remoteAddr = c.rwc.RemoteAddr().String()
	defer func() {
		if err := recover(); err != nil && err != ErrAbortHandler {
			const size = 64 << 10
			buf := make([]byte, size)
			buf = buf[:runtime.Stack(buf, false)]
			c.server.logf("http: panic serving %v: %v\n%s", c.remoteAddr, err, buf)
		}
		if !c.hijacked() {
			c.close()
			c.setState(c.rwc, StateClosed)
		}
	}()

	if tlsConn, ok := c.rwc.(*tls.Conn); ok {
		if d := c.server.ReadTimeout; d != 0 {
			c.rwc.SetReadDeadline(time.Now().Add(d))
		}
		if d := c.server.WriteTimeout; d != 0 {
			c.rwc.SetWriteDeadline(time.Now().Add(d))
		}
		if err := tlsConn.Handshake(); err != nil {
			c.server.logf("http: TLS handshake error from %s: %v", c.rwc.RemoteAddr(), err)
			return
		}
		c.tlsState = new(tls.ConnectionState)
		*c.tlsState = tlsConn.ConnectionState()
		if proto := c.tlsState.NegotiatedProtocol; validNPN(proto) {
			if fn := c.server.TLSNextProto[proto]; fn != nil {
				h := initNPNRequest{tlsConn, serverHandler{c.server}}
				fn(c.server, tlsConn, h)
			}
			return
		}
	}

	// HTTP/1.x from here on.

	ctx, cancelCtx := context.WithCancel(ctx)
	c.cancelCtx = cancelCtx
	defer cancelCtx()

	c.r = &connReader{conn: c}
	c.bufr = newBufioReader(c.r)
	c.bufw = newBufioWriterSize(checkConnErrorWriter{c}, 4<<10)

	for {
		w, err := c.readRequest(ctx)
		if c.r.remain != c.server.initialReadLimitSize() {
			// If we read any bytes off the wire, we're active.
			c.setState(c.rwc, StateActive)
		}
		if err != nil {
			const errorHeaders = "\r\nContent-Type: text/plain; charset=utf-8\r\nConnection: close\r\n\r\n"

			if err == errTooLarge {
				// Their HTTP client may or may not be
				// able to read this if we're
				// responding to them and hanging up
				// while they're still writing their
				// request. Undefined behavior.
				const publicErr = "431 Request Header Fields Too Large"
				fmt.Fprintf(c.rwc, "HTTP/1.1 "+publicErr+errorHeaders+publicErr)
				c.closeWriteAndWait()
				return
			}
			if isCommonNetReadError(err) {
				return // don't reply
			}

			publicErr := "400 Bad Request"
			if v, ok := err.(badRequestError); ok {
				publicErr = publicErr + ": " + string(v)
			}

			fmt.Fprintf(c.rwc, "HTTP/1.1 "+publicErr+errorHeaders+publicErr)
			return
		}

		// Expect 100 Continue support
		req := w.req
		if req.expectsContinue() {
			if req.ProtoAtLeast(1, 1) && req.ContentLength != 0 {
				// Wrap the Body reader with one that replies on the connection
				req.Body = &expectContinueReader{readCloser: req.Body, resp: w}
			}
		} else if req.Header.get("Expect") != "" {
			w.sendExpectationFailed()
			return
		}

		c.curReq.Store(w)

		if requestBodyRemains(req.Body) {
			registerOnHitEOF(req.Body, w.conn.r.startBackgroundRead)
		} else {
			if w.conn.bufr.Buffered() > 0 {
				w.conn.r.closeNotifyFromPipelinedRequest()
			}
			w.conn.r.startBackgroundRead()
		}

		// HTTP cannot have multiple simultaneous active requests.[*]
		// Until the server replies to this request, it can't read another,
		// so we might as well run the handler in this goroutine.
		// [*] Not strictly true: HTTP pipelining. We could let them all process
		// in parallel even if their responses need to be serialized.
		// But we're not going to implement HTTP pipelining because it
		// was never deployed in the wild and the answer is HTTP/2.
		serverHandler{c.server}.ServeHTTP(w, w.req)
		w.cancelCtx()
		if c.hijacked() {
			return
		}
		w.finishRequest()
		if !w.shouldReuseConnection() {
			if w.requestBodyLimitHit || w.closedRequestBodyEarly() {
				c.closeWriteAndWait()
			}
			return
		}
		c.setState(c.rwc, StateIdle)
		c.curReq.Store((*response)(nil))

		if !w.conn.server.doKeepAlives() {
			// We're in shutdown mode. We might've replied
			// to the user without "Connection: close" and
			// they might think they can send another
			// request, but such is life with HTTP/1.1.
			return
		}

		if d := c.server.idleTimeout(); d != 0 {
			c.rwc.SetReadDeadline(time.Now().Add(d))
			if _, err := c.bufr.Peek(4); err != nil {
				return
			}
		}
		c.rwc.SetReadDeadline(time.Time{})
	}
}
```

请求处理，对应不同的处理 
```
func (sh serverHandler) ServeHTTP(rw ResponseWriter, req *Request) {
	handler := sh.srv.Handler
	if handler == nil {
		handler = DefaultServeMux
	}
	if req.RequestURI == "*" && req.Method == "OPTIONS" {
		handler = globalOptionsHandler{}
	}
	handler.ServeHTTP(rw, req)
}
```

```
// ServeHTTP serves JSON-RPC requests over HTTP.
func (srv *Server) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if r.ContentLength > maxHTTPRequestContentLength {
		http.Error(w,
			fmt.Sprintf("content length too large (%d>%d)", r.ContentLength, maxHTTPRequestContentLength),
			http.StatusRequestEntityTooLarge)
		return
	}
	w.Header().Set("content-type", "application/json")

	// create a codec that reads direct from the request body until
	// EOF and writes the response to w and order the server to process
	// a single request.
	codec := NewJSONCodec(&httpReadWriteNopCloser{r.Body, w})
	defer codec.Close()
	srv.ServeSingleRequest(codec, OptionMethodInvocation)
}
```
rpc请求的处理
```
func (s *Server) serveRequest(codec ServerCodec, singleShot bool, options CodecOption) error {
	var pend sync.WaitGroup

	defer func() {
		if err := recover(); err != nil {
			const size = 64 << 10
			buf := make([]byte, size)
			buf = buf[:runtime.Stack(buf, false)]
			log.Error(string(buf))
		}
		s.codecsMu.Lock()
		s.codecs.Remove(codec)
		s.codecsMu.Unlock()
	}()

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()

	// if the codec supports notification include a notifier that callbacks can use
	// to send notification to clients. It is thight to the codec/connection. If the
	// connection is closed the notifier will stop and cancels all active subscriptions.
	if options&OptionSubscriptions == OptionSubscriptions {
		ctx = context.WithValue(ctx, notifierKey{}, newNotifier(codec))
	}
	s.codecsMu.Lock()
	if atomic.LoadInt32(&s.run) != 1 { // server stopped
		s.codecsMu.Unlock()
		return &shutdownError{}
	}
	s.codecs.Add(codec)
	s.codecsMu.Unlock()

	// test if the server is ordered to stop
	for atomic.LoadInt32(&s.run) == 1 {
		reqs, batch, err := s.readRequest(codec)
		if err != nil {
			// If a parsing error occurred, send an error
			if err.Error() != "EOF" {
				log.Debug(fmt.Sprintf("read error %v\n", err))
				codec.Write(codec.CreateErrorResponse(nil, err))
			}
			// Error or end of stream, wait for requests and tear down
			pend.Wait()
			return nil
		}

		// check if server is ordered to shutdown and return an error
		// telling the client that his request failed.
		if atomic.LoadInt32(&s.run) != 1 {
			err = &shutdownError{}
			if batch {
				resps := make([]interface{}, len(reqs))
				for i, r := range reqs {
					resps[i] = codec.CreateErrorResponse(&r.id, err)
				}
				codec.Write(resps)
			} else {
				codec.Write(codec.CreateErrorResponse(&reqs[0].id, err))
			}
			return nil
		}
		// If a single shot request is executing, run and return immediately
		if singleShot {
			if batch {
				s.execBatch(ctx, codec, reqs)
			} else {
				s.exec(ctx, codec, reqs[0])
			}
			return nil
		}
		// For multi-shot connections, start a goroutine to serve and loop back
		pend.Add(1)

		go func(reqs []*serverRequest, batch bool) {
			defer pend.Done()
			if batch {
				s.execBatch(ctx, codec, reqs)
			} else {
				s.exec(ctx, codec, reqs[0])
			}
		}(reqs, batch)
	}
	return nil
}
```

具体执行处理handle处理函数
/Users/eric/go/src/github.com/myself659/go-ethereum/rpc/server.go
```
// exec executes the given request and writes the result back using the codec.
func (s *Server) exec(ctx context.Context, codec ServerCodec, req *serverRequest) {
	var response interface{}
	var callback func()
	if req.err != nil {
		response = codec.CreateErrorResponse(&req.id, req.err)
	} else {
		response, callback = s.handle(ctx, codec, req)
	}

	if err := codec.Write(response); err != nil {
		log.Error(fmt.Sprintf("%v\n", err))
		codec.Close()
	}

	// when request was a subscribe request this allows these subscriptions to be actived
	if callback != nil {
		callback()
	}
}
```

/Users/eric/go/src/github.com/myself659/go-ethereum/rpc/types.go 
如何保证执行 
在req过程通过反射找到对应函数进行执行


```
type BlockValidator struct {
	config *params.ChainConfig // Chain configuration options
	bc     *BlockChain         // Canonical block chain
	engine consensus.Engine    // Consensus engine used for validating
}
```

通过读源码解决问题  

以太坊出块速度  
记录交易所在的区块的高度，通过高度计算确认数 

api是如何定义 

### 函数指针定义也可以作为对象   

```
// The HandlerFunc type is an adapter to allow the use of
// ordinary functions as HTTP handlers. If f is a function
// with the appropriate signature, HandlerFunc(f) is a
// Handler that calls f.
type HandlerFunc func(ResponseWriter, *Request)

// ServeHTTP calls f(w, r).
func (f HandlerFunc) ServeHTTP(w ResponseWriter, r *Request) {
	f(w, r)
}
```

上面这个很有意思 

### 注册  


### 接口流程分析  


```
// GetBalance returns the amount of wei for the given address in the state of the
// given block number. The rpc.LatestBlockNumber and rpc.PendingBlockNumber meta
// block numbers are also allowed.
func (s *PublicBlockChainAPI) GetBalance(ctx context.Context, address common.Address, blockNr rpc.BlockNumber) (*big.Int, error) {
	state, _, err := s.b.StateAndHeaderByNumber(ctx, blockNr)
	if state == nil || err != nil {
		return nil, err
	}
	b := state.GetBalance(address)
	return b, state.Error()
}
```
分析代码干的第一件事情是什么？

找到相关代码  


```
func (b *EthApiBackend) GetReceipts(ctx context.Context, blockHash common.Hash) (types.Receipts, error) {
	return core.GetBlockReceipts(b.eth.chainDb, blockHash, core.GetBlockNumber(b.eth.chainDb, blockHash)), nil
}
```

```
// GetTransactionReceipt returns the transaction receipt for the given transaction hash.
func (s *PublicTransactionPoolAPI) GetTransactionReceipt(hash common.Hash) (map[string]interface{}, error) {
	receipt := core.GetReceipt(s.b.ChainDb(), hash)
	if receipt == nil {
		log.Debug("Receipt not found for transaction", "hash", hash)
		return nil, nil
	}

	tx, _, err := getTransaction(s.b.ChainDb(), s.b, hash)
	if err != nil {
		log.Debug("Failed to retrieve transaction", "hash", hash, "err", err)
		return nil, nil
	}

	txBlock, blockIndex, index, err := getTransactionBlockData(s.b.ChainDb(), hash)
	if err != nil {
		log.Debug("Failed to retrieve transaction block", "hash", hash, "err", err)
		return nil, nil
	}

	var signer types.Signer = types.FrontierSigner{}
	if tx.Protected() {
		signer = types.NewEIP155Signer(tx.ChainId())
	}
	from, _ := types.Sender(signer, tx)

	fields := map[string]interface{}{
		"root":              hexutil.Bytes(receipt.PostState),
		"blockHash":         txBlock,
		"blockNumber":       hexutil.Uint64(blockIndex),
		"transactionHash":   hash,
		"transactionIndex":  hexutil.Uint64(index),
		"from":              from,
		"to":                tx.To(),
		"gasUsed":           (*hexutil.Big)(receipt.GasUsed),
		"cumulativeGasUsed": (*hexutil.Big)(receipt.CumulativeGasUsed),
		"contractAddress":   nil,
		"logs":              receipt.Logs,
		"logsBloom":         receipt.Bloom,
	}
	if receipt.Logs == nil {
		fields["logs"] = [][]*types.Log{}
	}
	// If the ContractAddress is 20 0x0 bytes, assume it is not a contract creation
	if receipt.ContractAddress != (common.Address{}) {
		fields["contractAddress"] = receipt.ContractAddress
	}
	return fields, nil
}
```

### 交易状态的pending的确认 

是从交易池中获取的交易为pending状态  


### 解析erc20数据 

erc20代币发现

解析代码  

从代码里找到答案  



  ;; Memory layout.
  (def 'mem-ret    0x00) ; Fixed due to compiler macro for return.
  (def 'mem-func   0x00) ; No conflict with mem-ret, so re-use.
  (def 'mem-keccak 0x00) ; No conflict with mem-func or mem-ret, so re-use.
  (def 'scratch0   0x20)
  (def 'scratch1   0x40)
  
  ;; Precomputed function IDs.
  (def 'get-name         0x06fdde03) ; name()
  (def 'get-symbol       0x95d89b41) ; symbol()
  (def 'get-decimals     0x313ce567) ; decimals()
  (def 'get-total-supply 0x18160ddd) ; totalSupply()
  (def 'get-balance-of   0x70a08231) ; balanceOf(address)
  (def 'transfer         0xa9059cbb) ; transfer(address,uint256)
  (def 'transfer-from    0x23b872dd) ; transferFrom(address,address,uint256)
  (def 'approve          0x095ea7b3) ; approve(address,uint256)
  (def 'get-allowance    0xdd62ed3e) ; allowance(address,address)  ;; Memory layout.
  (def 'mem-ret    0x00) ; Fixed due to compiler macro for return.
  (def 'mem-func   0x00) ; No conflict with mem-ret, so re-use.
  (def 'mem-keccak 0x00) ; No conflict with mem-func or mem-ret, so re-use.
  (def 'scratch0   0x20)
  (def 'scratch1   0x40)
  
  ;; Precomputed function IDs.
  (def 'get-name         0x06fdde03) ; name()
  (def 'get-symbol       0x95d89b41) ; symbol()
  (def 'get-decimals     0x313ce567) ; decimals()
  (def 'get-total-supply 0x18160ddd) ; totalSupply()
  (def 'get-balance-of   0x70a08231) ; balanceOf(address)
  (def 'transfer         0xa9059cbb) ; transfer(address,uint256)
  (def 'transfer-from    0x23b872dd) ; transferFrom(address,address,uint256)
  (def 'approve          0x095ea7b3) ; approve(address,uint256)
  (def 'get-allowance    0xdd62ed3e) ; allowance(address,address)

用于传入参数  

data: DATA - (optional) The compiled code of a contract  


curl -i -X POST \
   -H "Content-Type:application/json" \
   -d \
'{"jsonrpc":"2.0","method":"eth_call","params":[{"to":"0x937ea0eed76dc56a1862d473f6b5dbf6ea1df191","data":"0x95d89b41"}, "latest"],"id":1}' \
 'http://47.52.31.232:8545'
 

精度  

curl -i -X POST \
   -H "Content-Type:application/json" \
   -d \
'{"jsonrpc":"2.0","method":"eth_call","params":[{"to":"0x937ea0eed76dc56a1862d473f6b5dbf6ea1df191","data":"0x313ce567"}, "latest"],"id":1}' \
 'http://47.52.31.232:8545'  



