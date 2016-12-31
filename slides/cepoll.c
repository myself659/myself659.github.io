int RtmpSessionHandle(int iFd, int iEvent, void *pContext)
{
    int iRet;
    RTMP_SESSION *pSession = (RTMP_SESSION *)pContext;
    
    if(iEvent&EPOLLIN )
    {
        if(0 == pSession->handshake)
        {
            iRet = RtmpSessionHandshake(pSession);      
            if(0 != iRet)
            {
                RtmpSessionHandleFin(pSession); 
            }
        }
        else
        {   
            iRet =  RtmpPktHandle(pSession);
        }
    }
    if(iEvent & (EPOLLERR |EPOLLHUP) )
    {
        RtmpSessionHandleFin(pSession);
    }
        
    return iRet;
}




int ListenHandle(int iFd, int iEvent, void *pContext)
{
    int iNewFd;
    int iRet = 0;
    struct sockaddr tmpAddr;
    memset(&tmpAddr, 0, sizeof(tmpAddr));
    int iSocketSize = sizeof(tmpAddr);
    EPOLL_CTX *pCtx; 
    RTMP_SESSION *pServer;
    
    if(iEvent|EPOLLIN)
    {
        iNewFd = accept(iFd, &tmpAddr, (socklen_t *)&iSocketSize); 
        if(RTMP_EPOLLSRV_INVALIDFD < iNewFd)    
        {
            pServer = (RTMP_SESSION *)malloc(sizeof(RTMP_SESSION));
            if(NULL == pServer)
            {
                return -1;
            }
            pServer->handshake  = 0; 
            pCtx = (EPOLL_CTX *)malloc(sizeof(EPOLL_CTX));
            if(NULL == pCtx)
            {
                free(pServer);
                return -1;
            }
            pServer->socket = iNewFd;
            pCtx->iFd = iNewFd;
            pCtx->pContext = pServer;
            pCtx->pfHandle = RtmpSessionHandle;
            /* 加入epoll */
            iRet = epoll_op(g_iEpollFd, EPOLL_CTL_ADD, iNewFd,  EPOLLIN|EPOLLERR|EPOLLHUP,  pCtx);
            
        }
        else
        {
            printf("accept errno:%s",strerror(errno));
        }
    }

    return iRet;
}


int epoll_op(int iEpollFd, int iOp, int iFd, int iEvent,  EPOLL_CTX *pCtx)
{
    int iRet;
    struct epoll_event ev;

    ev.events = iEvent;
    ev.data.ptr = pCtx;
    
    iRet = epoll_ctl(iEpollFd, iOp, iFd, &ev);
    
    return iRet;
}


int epoll_loop(int iEpollFd)
{
    int iNum;
    struct epoll_event astEpEvent[RTMP_EPOLLSRV_MAXEPOLL];
    int i;
    EpollCallBack_PF pfHandle;
    EPOLL_CTX *pCtx;
    for( ;  ;)
    {
        iNum= epoll_wait(iEpollFd, &astEpEvent[0],  RTMP_EPOLLSRV_MAXEPOLL, -1);
        if( 0 < iNum)
        {
            for(i = 0; i < iNum; i++)
            {
                pCtx = (EPOLL_CTX *)astEpEvent[i].data.ptr;
                pfHandle = pCtx->pfHandle;
                (void)pfHandle(pCtx->iFd, astEpEvent[i].events, pCtx->pContext);
            }
        }
        else
        {
            printf("epoll_wait failed\r\n");
        }
    }

    return 0;
}

int main(void)
{
    int iFd;
    struct sockaddr_in addr;
    
    printf("in the main\r\n");
    /* 初始化epoll */
    g_iEpollFd = epoll_create(200);
    if(RTMP_EPOLLSRV_INVALIDFD >= g_iEpollFd)
    {
        printf("create epoll failed\r\n");
        return  -1;
    }

    
    /* 创建侦听端口 */
    iFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(RTMP_EPOLLSRV_INVALIDFD >= iFd)
    {
        printf("create listen socket failed\r\n");
        return  -1;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(g_cRtmpSrvAddr);
    addr.sin_port = htons(g_usRtmpSrvPort);

    if( 0 != bind(iFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)))
    {
        return  -1;  
    }


    if( 0 != listen(iFd, 200))
    {
        return  -1;
    }

    EPOLL_CTX *pEpollCtx = (EPOLL_CTX *)malloc(sizeof(EPOLL_CTX));
    if(NULL == pEpollCtx)
    {
        return -1;
    }

    pEpollCtx->iFd = iFd;
    pEpollCtx->pfHandle = ListenHandle;
    pEpollCtx->pContext = NULL;
    /* 加入epoll */
    if(0 != epoll_op(g_iEpollFd, EPOLL_CTL_ADD, iFd, EPOLLIN|EPOLLERR|EPOLLHUP, pEpollCtx))
    {
        return -1;
    }

    g_iListenFd = iFd;

    epoll_loop(g_iEpollFd);
    

    
    return  0;
    
}