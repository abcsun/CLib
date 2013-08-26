#include "include/socket_base.h"
#include "include/socket_addr.h"
#include "include/log_trace.h"
#include <string>

using namespace std;

CommBase::CommBase()
{
  sockFd = -1;
}

CommBase::~CommBase()
{
  close(sockFd);
  sockFd = -1;
}

int CommBase::getSockFd()
{
  return sockFd;
}

void CommBase::setSockFd(int fd)
{
  sockFd = fd;
}
int CommBase::acceptSock(SockAddr &addr_)
{
  struct sockaddr *sockAddr = addr_.getAddr();
  socklen_t addrLen = addr_.getSockLen();

  int connFd = accept(sockFd, sockAddr, &addrLen);
  if(-1 == connFd)
  {
    logTrace("%m");
  }

  return connFd;
}
bool CommBase::connectTo(SockAddr &addr_)
{
  sockFd = socket(addr_.getDomain(), addr_.getType(), 0);
  if( -1 == sockFd)
  {
    logTrace("%m");
    return false;
  }
  
  const struct sockaddr *sockAddr = addr_.getAddr();
  if(NULL == sockAddr)
  {
    logTrace("");
    return false;
  }

  int rc = connect(sockFd, sockAddr, addr_.getSockLen());
  if(-1 == rc)
  {
    logTrace("%m");
    return false;
  }

  return true;
}

int CommBase::sendData(void *data_, 
    uint32_t len_)
{
  logTrace("you can't use this method");

  return -1;
}

int CommBase::sendData(void *data_, 
    uint32_t len_, 
    SockAddr &addr_)
{
  logTrace("you can't use this method");

  return -1;
}

int CommBase::recvData(void *data_, 
    uint32_t len_)
{
  logTrace("you can't use this method");

  return -1;
}

int CommBase::recvData(void *data_, 
    uint32_t len_,
    SockAddr &addr_)
{
  logTrace("you can't use this method");

  return -1;
}
      
bool CommBase::enableSockReuse()
{
  int flag = 1;
  int rc = setsockopt (sockFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag));
  if(0 != rc)
  {
    logTrace("%m");
  }

  return (0 == rc);
}

bool CommBase::setSendBufSize(uint32_t size)
{
  int rc = setsockopt(sockFd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
  if(0 != rc)
  {
    logTrace("%m");
  }

  return (0 == rc);
}

bool CommBase::setRecvBufSize(uint32_t size)
{
  int rc = setsockopt(sockFd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
  if(0 != rc)
  {
    logTrace("%m");
  }

  return (0 == rc);
}

int CommBase::getSendBufSize()
{
  int size;
  socklen_t len = sizeof(size);
  int rc = getsockopt(sockFd, SOL_SOCKET, SO_SNDBUF, &size, &len );
  if(0 != rc)
  {
    logTrace("%m");
    return -1;
  }

  return size;
}

int CommBase::getRecvBufSize()
{
  int size;
  socklen_t len = sizeof(size);
  int rc = getsockopt(sockFd, SOL_SOCKET, SO_RCVBUF, &size, &len );
  if(0 != rc)
  {
    logTrace("%m");
    return -1;
  }

  return size;
}

StreamSock::StreamSock()
{
  //do nothing
}
StreamSock::~StreamSock()
{
  //do nothing
}
bool StreamSock::openSock(SockAddr &addr_)
{
  sockFd = socket(addr_.getDomain(), addr_.getType(), 0);
  if( -1 == sockFd)
  {
    logTrace("%m");
    return false;
  }

  enableSockReuse();
  
  const struct sockaddr *sockAddr = addr_.getAddr();
  if(NULL == sockAddr)
  {
    logTrace("");
    return false;
  }

  int ret = bind(sockFd, sockAddr, addr_.getSockLen());
  if(0 != ret)
  {
    logTrace("%m");
    return false;
  }

  ret = listen(sockFd, 32);
  if(0 != ret)
  {
    logTrace("%m");
    return false;
  }

  return true;
}
int StreamSock::sendData(void *data_, 
    uint32_t len_)
{
  int sendLen = 0;
  do
  {
    sendLen = send(sockFd, data_, len_, 0);
  }while(-1 == sendLen && EINTR == errno);

  if(sendLen <= 0)
  {
    logTrace("%m");
  }

  return sendLen;
}

int StreamSock::recvData(void *data_, 
    uint32_t len_)
{
  int recvLen = 0;
  do
  {
    recvLen = recv(sockFd, data_, len_, 0);
  }while(-1 == recvLen && EINTR == errno);

  if(recvLen <= 0)
  {
    logTrace("%m");
  }

  return recvLen;
}
DgramSock::DgramSock()
{
  //do nothing
}
DgramSock::~DgramSock()
{
  //do nothing
}
bool DgramSock::openSock(SockAddr &addr_)
{
  sockFd = socket(addr_.getDomain(), addr_.getType(), 0);
  if( -1 == sockFd)
  {
    logTrace("%m");
    return false;
  }

  
  const struct sockaddr *sockAddr = addr_.getAddr();
  if(NULL == sockAddr)
  {
    logTrace("");
    return false;
  }

  int ret = bind(sockFd, sockAddr, addr_.getSockLen());
  if(0 != ret)
  {
    logTrace("%m");
    return false;
  }

  return true;
}
int DgramSock::sendData(void *data_, 
    uint32_t len_, 
    SockAddr &addr_)
{
  const struct sockaddr *sockAddr = addr_.getAddr();
  
  if(NULL == sockAddr)
  {
    logTrace("");
    return false;
  }

  int sendLen = 0;
  do
  {
    sendLen = sendto(sockFd, data_, len_, 0, sockAddr, addr_.getSockLen());
  }while(-1 == sendLen && EINTR == errno);

  if(sendLen <= 0)
  {
    logTrace("%m");
  }

  return sendLen;
}
int DgramSock::recvData(void *data_, 
    uint32_t len_,
    SockAddr &addr_)
{
  struct sockaddr *sockAddr = addr_.getAddr();
  socklen_t addrLen = addr_.getSockLen();
  
  int recvLen = 0;
  do
  {
    recvLen = recvfrom(sockFd, data_, len_, 0, sockAddr, &addrLen);
  }while(-1 == recvLen && EINTR == errno);

  if(recvLen <= 0)
  {
    logTrace("%m");
  }

  return recvLen;
}
