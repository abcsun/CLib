/***************************************************************************************
****************************************************************************************
* FILE		: socket.cc
* Description	: 
*			  
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
*            Without permission, shall not be used for any commercial purpose
* 
* History:
* Version		Name       		Date			Description
   0.1		Liu Yanyun		2012/12/11		Initial Version
   
****************************************************************************************
****************************************************************************************/


#include "include/socket.h"
#include "include/sock_ev.h"
#include "include/socket_addr.h"
#include "include/socket_base.h"
#include "include/log_trace.h"

using namespace std;

Socket::Socket()
{
  evt  = 0;
  comm = NULL;
  addr = NULL;
  loop = NULL;
  rdCb = NULL;
  wrCb = NULL;
  rdArg= NULL;
  wrArg= NULL;
}

Socket::~Socket()
{
  if(NULL != comm) delete comm;
  
  if(NULL != addr) delete addr;
}

Socket* Socket::create()
{
  Socket *sock = new Socket();
  if(NULL == sock)
  {
    logTrace("new Socket() failed");
    return NULL;
  }

  return sock;
}
void Socket::destroy(Socket* &sock_)
{
  if(NULL != sock_) delete sock_;

  sock_ = NULL;
}
int Socket::getFd()
{
  return comm->getSockFd();
}
bool Socket::open(const char *uri_)
{
  addr = new SockAddr(uri_);
  if(NULL == addr)
  {
    logTrace("new SockAddr(%s) failed", uri_);
    return false;
  }

  if(!addr->parseUri())
  {
    logTrace("parseUri() failed;uri:%s", uri_);
    return false;
  }
  
  int type = addr->getType();
  if(SOCK_STREAM == type)
  {
    comm = new StreamSock();
  }
  else if(SOCK_DGRAM == type)
  {
    comm = new DgramSock();
  }
  else
  {
    logTrace("addr.type is invalid;type:%s", type);
    return false;
  }

  if(NULL == comm)
  {
    logTrace("new StreamSock() failed");
    return false;
  }
  
  if(!comm->openSock(*addr))
  {
    logTrace("StreamSock.openSock() failed");
    return false;
  }

  return true;
}
bool Socket::connect(const char *uri_)
{
  addr = new SockAddr(uri_);
  if(NULL == addr)
  {
    logTrace("new SockAddr(%s) failed", uri_);
    return false;
  }

  if(!addr->parseUri())
  {
    logTrace("parseUri() failed;uri:%s", uri_);
    return false;
  }
  
  int type = addr->getType();
  if(SOCK_STREAM == type)
  {
    comm = new StreamSock();
  }
  else if(SOCK_DGRAM == type)
  {
    comm = new DgramSock();
  }
  else
  {
    logTrace("addr.type is invalid;type:%s", type);
    return false;
  }

  if(NULL == comm)
  {
    logTrace("new StreamSock() failed");
    return false;
  }
  
  if(!comm->connectTo(*addr))
  {
    logTrace("StreamSock.connectTo() failed");
    return false;
  }

  return true;
}
bool Socket::accept(Socket *sock_)
{
  sock_->addr = new SockAddr(addr->getDomain(), addr->getType());
  if(NULL == sock_->addr)
  {
    logTrace("new SockAddr(%d, %d) failed", addr->getDomain(), addr->getType());
    return false;
  }

  int acceptFd = comm->acceptSock(*(sock_->addr));
  if(-1 == acceptFd)
  {
    logTrace("accetp connection is failed");
    return false;
  }

  int type = addr->getType();
  if(SOCK_STREAM == type)
  {
    sock_->comm = new StreamSock();
  }
  else if(SOCK_DGRAM == type)
  {
    sock_->comm = new DgramSock();
  }
  else
  {
    logTrace("addr.type is invalid;type:%s", type);
    return false;
  }

  if(NULL == sock_->comm)
  {
    logTrace("new StreamSock() failed");
    return false;
  }
  
  sock_->comm->setSockFd(acceptFd);

  return true;
  
}
int Socket::send(void *data_, 
    uint32_t len_)
{
  return comm->sendData(data_, len_);
}
int Socket::send(void *data_, 
    uint32_t len_, 
    const char *to_)
{
  SockAddr *tmpAddr = new SockAddr(to_);
  if(NULL == tmpAddr)
  {
    logTrace("new SockAddr(%s) failed", to_);
    return -1;
  }

  if(!tmpAddr->parseUri())
  {
    logTrace("parseUri() failed;uri:%s", to_);
    delete tmpAddr;
    return false;
  }

  int sendLen = comm->sendData(data_, len_, *tmpAddr);

  delete tmpAddr;

  return sendLen;
}
int Socket::recv(void *data_, 
    uint32_t len_)
{
  return comm->recvData(data_, len_);
}
int Socket::recv(void *data_, 
    uint32_t len_,
    char *from_)
{
  SockAddr *tmpAddr = new SockAddr(addr->getDomain(), addr->getType());
  if(NULL == tmpAddr)
  {
    logTrace("new SockAddr(%s) failed");
    return -1;
  }

  int sendLen = comm->recvData(data_, len_, *tmpAddr);

  if(NULL != from_)
  {
    string tmpStr;
    if(tmpAddr->toStr(tmpStr))
    {
      sprintf(from_, "%s", tmpStr.c_str());
    }
    else
    {
      sprintf(from_, "%s", "Invalid socket address string");
    }
  }

  delete tmpAddr;

  return sendLen;
}
EventType Socket::getEvt()
{
  return evt;
}
void Socket::processEvent(EventType evt_)
{
  if(NULL != rdCb && evt_&evRead)
  {
    rdCb(loop, this, evRead, rdArg);
  }
  else if(NULL != wrCb && evt_&evWrite)
  {
    wrCb(loop, this, evWrite, wrArg);
  }
  else
  {
    logTrace("eventType:%d;reCb:%p,wrCb:%p", evt_, rdCb, wrCb);
  }
}

void Socket::setCallBack(EventLoop *loop_, 
    EvCallBack cb_,
    EventType evt_,
    void *arg_)
{
  loop = loop_;
  
  if(evt_& evRead)
  {
    evt |= evRead;
    rdCb = cb_;
    rdArg= arg_;
  }

  if(evt_& evWrite)
  {
    evt |= evWrite;
    wrCb = cb_;
    wrArg= arg_;
  }

}

void Socket::clearCallBack(EventType evt_)
{
  if(evt_&evRead)
  {
    evt &= ~evRead;
    rdCb = NULL;
    rdArg= NULL;
  }

  if(evt_&evWrite)
  {
    evt &= ~evRead;
    wrCb = NULL;
    wrArg= NULL;
  }

  if(0 == evt)
  {
    loop = NULL;
  }
}
