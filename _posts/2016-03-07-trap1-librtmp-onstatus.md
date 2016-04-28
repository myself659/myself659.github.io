---
layout: post
title:  那些坑事
categories: 那些坑事：Librtmp的onstatus报文格式错误
tags: 那些坑事 
---

### 前言
写这个的目的，除了记录一下自己经历的一些坑事的经历，也希望能够将这些坑公开出来，让后面的人能不再踩这个坑

### 踩坑 

参考librtmp源码rtmpsrv.c自己写一个rtmpserver的demo程序，但是发布不成功，具体表现为发布客户端总是主动关闭连接。


### 爬坑

第一步：

源码面前无秘密，替换了flash 发布客户端，自己改代码实现一个客户端，测试发现还是一样；
第二步： 发布客户端与服务端代码都是白盒，那就分析代码吧 

客户端onstatus命令处理：
```
/* Returns 0 for OK/Failed/error, 1 for 'Stop or Complete' */
static int
HandleInvoke(RTMP *r, const char *body, unsigned int nBodySize)
{
  AMFObject obj;
  AVal method;
  double txn;
  int ret = 0, nRes;
  if (body[0] != 0x02)    /* make sure it is a string method name we start with */
  {
    RTMP_Log(RTMP_LOGWARNING, "%s, Sanity failed. no string method in invoke packet",
             __FUNCTION__);
    return 0;
  }

  nRes = AMF_Decode(&obj, body, nBodySize, FALSE); /* AMF解析object */
  if (nRes < 0)
  {
    RTMP_Log(RTMP_LOGERROR, "%s, error decoding invoke packet", __FUNCTION__);
    return 0;
  }

  AMF_Dump(&obj); /* dump amf 信息 */
  AMFProp_GetString(AMF_GetProp(&obj, NULL, 0), &method);
  txn = AMFProp_GetNumber(AMF_GetProp(&obj, NULL, 1));
  RTMP_Log(RTMP_LOGDEBUG, "%s, server invoking <%s>", __FUNCTION__, method.av_val);

  if (AVMATCH(&method, &av__result))
  {
    AVal methodInvoked = {0};
    int i;

    for (i = 0; i < r->m_numCalls; i++) {
      if (r->m_methodCalls[i].num == (int)txn) {
        methodInvoked = r->m_methodCalls[i].name;
        AV_erase(r->m_methodCalls, &r->m_numCalls, i, FALSE);
        break;
      }
    }
    if (!methodInvoked.av_val) {
      RTMP_Log(RTMP_LOGDEBUG, "%s, received result id %f without matching request",
               __FUNCTION__, txn);
      goto leave;
    }

    RTMP_Log(RTMP_LOGDEBUG, "%s, received result for method call <%s>", __FUNCTION__,
             methodInvoked.av_val);

    if (AVMATCH(&methodInvoked, &av_connect))
    {
      if (r->Link.token.av_len)
      {
        AMFObjectProperty p;
        if (RTMP_FindFirstMatchingProperty(&obj, &av_secureToken, &p))
        {
          DecodeTEA(&r->Link.token, &p.p_vu.p_aval);
          SendSecureTokenResponse(r, &p.p_vu.p_aval);
        }
      }
      if (r->Link.protocol & RTMP_FEATURE_WRITE)
      {
        SendReleaseStream(r);
        SendFCPublish(r);
      }
      else
      {
        RTMP_SendServerBW(r);
        RTMP_SendCtrl(r, 3, 0, 300);
      }
      RTMP_SendCreateStream(r);

      if (!(r->Link.protocol & RTMP_FEATURE_WRITE))
      {
        /* Authenticate on Justin.tv legacy servers before sending FCSubscribe */
        if (r->Link.usherToken.av_len)
          SendUsherToken(r, &r->Link.usherToken);
        /* Send the FCSubscribe if live stream or if subscribepath is set */
        if (r->Link.subscribepath.av_len)
          SendFCSubscribe(r, &r->Link.subscribepath);
        else if (r->Link.lFlags & RTMP_LF_LIVE)
          SendFCSubscribe(r, &r->Link.playpath);
      }
    }
    else if (AVMATCH(&methodInvoked, &av_createStream))
    {
      r->m_stream_id = (int)AMFProp_GetNumber(AMF_GetProp(&obj, NULL, 3));

      if (r->Link.protocol & RTMP_FEATURE_WRITE)
      {
        SendPublish(r); 
      }
      else
      {
        if (r->Link.lFlags & RTMP_LF_PLST)
          SendPlaylist(r);
        SendPlay(r);
        RTMP_SendCtrl(r, 3, r->m_stream_id, r->m_nBufferMS);
      }
    }
    else if (AVMATCH(&methodInvoked, &av_play) ||
             AVMATCH(&methodInvoked, &av_publish))
    {
      r->m_bPlaying = TRUE;
    }
    free(methodInvoked.av_val);
  }
  else if (AVMATCH(&method, &av_onBWDone))
  {
    if (!r->m_nBWCheckCounter)
      SendCheckBW(r);
  }
  else if (AVMATCH(&method, &av_onFCSubscribe))
  {
    /* SendOnFCSubscribe(); */
  }
  else if (AVMATCH(&method, &av_onFCUnsubscribe))
  {
    RTMP_Close(r);
    ret = 1;
  }
  else if (AVMATCH(&method, &av_ping))
  {
    SendPong(r, txn);
  }
  else if (AVMATCH(&method, &av__onbwcheck))
  {
    SendCheckBWResult(r, txn);
  }
  else if (AVMATCH(&method, &av__onbwdone))
  {
    int i;
    for (i = 0; i < r->m_numCalls; i++)
      if (AVMATCH(&r->m_methodCalls[i].name, &av__checkbw))
      {
        AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
        break;
      }
  }
  else if (AVMATCH(&method, &av__error))
  {
#ifdef CRYPTO
    AVal methodInvoked = {0};
    int i;

    if (r->Link.protocol & RTMP_FEATURE_WRITE)
    {
      for (i = 0; i < r->m_numCalls; i++)
      {
        if (r->m_methodCalls[i].num == txn)
        {
          methodInvoked = r->m_methodCalls[i].name;
          AV_erase(r->m_methodCalls, &r->m_numCalls, i, FALSE);
          break;
        }
      }
      if (!methodInvoked.av_val)
      {
        RTMP_Log(RTMP_LOGDEBUG, "%s, received result id %f without matching request",
                 __FUNCTION__, txn);
        goto leave;
      }

      RTMP_Log(RTMP_LOGDEBUG, "%s, received error for method call <%s>", __FUNCTION__,
               methodInvoked.av_val);

      if (AVMATCH(&methodInvoked, &av_connect))
      {
        AMFObject obj2;
        AVal code, level, description;
        AMFProp_GetObject(AMF_GetProp(&obj, NULL, 3), &obj2);
        AMFProp_GetString(AMF_GetProp(&obj2, &av_code, -1), &code);
        AMFProp_GetString(AMF_GetProp(&obj2, &av_level, -1), &level);
        AMFProp_GetString(AMF_GetProp(&obj2, &av_description, -1), &description);
        RTMP_Log(RTMP_LOGDEBUG, "%s, error description: %s", __FUNCTION__, description.av_val);
        /* if PublisherAuth returns 1, then reconnect */
        if (PublisherAuth(r, &description) == 1)
        {
          CloseInternal(r, 1);
          if (!RTMP_Connect(r, NULL) || !RTMP_ConnectStream(r, 0))
            goto leave;
        }
      }
    }
    else
    {
      RTMP_Log(RTMP_LOGERROR, "rtmp server sent error");
    }
    free(methodInvoked.av_val);
#else
    RTMP_Log(RTMP_LOGERROR, "rtmp server sent error");
#endif
  }
  else if (AVMATCH(&method, &av_close))
  {
    RTMP_Log(RTMP_LOGERROR, "rtmp server requested close");
    RTMP_Close(r);
  }
  else if (AVMATCH(&method, &av_onStatus))  /* 处理onstatus命令 */
  {
    AMFObject obj2;
    AVal code, level;
    AMFProp_GetObject(AMF_GetProp(&obj, NULL, 3), &obj2);  /* 获取第4个obj */
    AMFProp_GetString(AMF_GetProp(&obj2, &av_code, -1), &code); /* 从子obj获取code信息 */
    AMFProp_GetString(AMF_GetProp(&obj2, &av_level, -1), &level);

    RTMP_Log(RTMP_LOGDEBUG, "%s, onStatus: %s", __FUNCTION__, code.av_val);
    if (AVMATCH(&code, &av_NetStream_Failed)
        || AVMATCH(&code, &av_NetStream_Play_Failed)
        || AVMATCH(&code, &av_NetStream_Play_StreamNotFound)
        || AVMATCH(&code, &av_NetConnection_Connect_InvalidApp))
    {
      r->m_stream_id = -1;
      RTMP_Close(r);
      RTMP_Log(RTMP_LOGERROR, "Closing connection: %s", code.av_val);
    }

    else if (AVMATCH(&code, &av_NetStream_Play_Start)
             || AVMATCH(&code, &av_NetStream_Play_PublishNotify))
    {
      int i;
      r->m_bPlaying = TRUE;
      for (i = 0; i < r->m_numCalls; i++)
      {
        if (AVMATCH(&r->m_methodCalls[i].name, &av_play))
        {
          AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
          break;
        }
      }
    }

    else if (AVMATCH(&code, &av_NetStream_Publish_Start)) 
    {
      int i;
      r->m_bPlaying = TRUE;
      for (i = 0; i < r->m_numCalls; i++)
      {
        if (AVMATCH(&r->m_methodCalls[i].name, &av_publish))
        {
          AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
          break;
        }
      }
    }

    /* Return 1 if this is a Play.Complete or Play.Stop */
    else if (AVMATCH(&code, &av_NetStream_Play_Complete)
             || AVMATCH(&code, &av_NetStream_Play_Stop)
             || AVMATCH(&code, &av_NetStream_Play_UnpublishNotify))
    {
      RTMP_Close(r);
      ret = 1;
    }

    else if (AVMATCH(&code, &av_NetStream_Seek_Notify))
    {
      r->m_read.flags &= ~RTMP_READ_SEEKING;
    }

    else if (AVMATCH(&code, &av_NetStream_Pause_Notify))
    {
      if (r->m_pausing == 1 || r->m_pausing == 2)
      {
        RTMP_SendPause(r, FALSE, r->m_pauseStamp);
        r->m_pausing = 3;
      }
    }
  }
  else if (AVMATCH(&method, &av_playlist_ready))
  {
    int i;
    for (i = 0; i < r->m_numCalls; i++)
    {
      if (AVMATCH(&r->m_methodCalls[i].name, &av_set_playlist))
      {
        AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
        break;
      }
    }
  }
  else
  {

  }
leave:
  AMF_Reset(&obj);
  return ret;
}
```
从上面的客户端代码，可以看出来需要的onstatus报文格式如下：


对应服务端代码
```
static int
SendPlayStart(RTMP *r)
{
  RTMPPacket packet;
  char pbuf[512], *pend = pbuf + sizeof(pbuf);

  packet.m_nChannel = 0x03;     // control channel (invoke)
  packet.m_headerType = 1; /* RTMP_PACKET_SIZE_MEDIUM; */
  packet.m_packetType = RTMP_PACKET_TYPE_INVOKE;
  packet.m_nTimeStamp = 0;
  packet.m_nInfoField2 = 0;
  packet.m_hasAbsTimestamp = 0;
  packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

  char *enc = packet.m_body;
  enc = AMF_EncodeString(enc, pend, &av_onStatus);
  enc = AMF_EncodeNumber(enc, pend, 0);  
  /* 无Command object */
  *enc++ = AMF_OBJECT;

  enc = AMF_EncodeNamedString(enc, pend, &av_level, &av_status); 
  enc = AMF_EncodeNamedString(enc, pend, &av_code, &av_NetStream_Play_Start);
  enc = AMF_EncodeNamedString(enc, pend, &av_description, &av_Started_playing);
  enc = AMF_EncodeNamedString(enc, pend, &av_details, &r->Link.playpath);
  enc = AMF_EncodeNamedString(enc, pend, &av_clientid, &av_clientid);
  *enc++ = 0;
  *enc++ = 0;
  *enc++ = AMF_OBJECT_END;

  packet.m_nBodySize = enc - packet.m_body;
  return RTMP_SendPacket(r, &packet, FALSE);
}
```


第三步：
回头再确认一下rtmp规范，具体规范如下：



确认问题原因了，接下来就简单了



### 总结 
1. 对开源代码保持一颗敬畏之心，不轻视也不迷信，吃透代码，好办事
2. 对于librtmp的代码确实有很多在用，但是基本是用于客户端的场景，用作服务端的场景基本不可见，所以除了RTMP协议实现的代码是有实践保障，其他的代码基本只能仅作参考 
3.

### 参考
1. [rtmp_specification_1.0](https://www.adobe.com/content/dam/Adobe/en/devnet/rtmp/pdf/rtmp_specification_1.0.pdf)
2. https://rtmpdump.mplayerhq.hu/ 