/***************************************************************************************
****************************************************************************************
* FILE		: socket_base.h
* Description	: 
*			  
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
*            Without permission, shall not be used for any commercial purpose
* 
* History:
* Version		Name       		Date			Description
   0.1		Liu Yanyun		2012/12/06		Initial Version
   
****************************************************************************************
****************************************************************************************/


#ifndef _SOCKET_BASE_H_
#define _SOCKET_BASE_H_


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string>

class SockAddr;


/*==================================================================
* Function	: CommBase
* Description	: socket base class
==================================================================*/
class CommBase
{
public:

  /*==================================================================
  * Function	: CommBase
  * Description	: construction function
  ==================================================================*/
  CommBase();

  /*==================================================================
  * Function	: CommBase
  * Description	: Destructor function
  ==================================================================*/
  virtual ~CommBase();

  /*==================================================================
  * Function	: CommBase.getSockFd
  * Description	: get socket fd
  * Return Value: socket fd
  ==================================================================*/
  int getSockFd();

  /*==================================================================
  * Function	: CommBase.setSockFd
  * Description	: set socket fd
  * Input Para	: socket fd
  * Return Value: void
  ==================================================================*/
  void setSockFd(int fd);

  /*==================================================================
  * Function	: CommBase.openSock
  * Description	: for stream (socket->bind->listen);for dgram(socket->bind)
  * Input Para	: socket address
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  virtual bool openSock(SockAddr &addr_) = 0;

  /*==================================================================
  * Function	: CommBase.acceptSock
  * Description	: used for server accept client connection
  * Input Para	: addr_--socket address
  * Output Para	: addr_--client address
  * Return Value: if success return client fd, or else -1 is return
  ==================================================================*/
  int acceptSock(SockAddr &addr_);

  /*==================================================================
  * Function	: CommBase.connectTo
  * Description	: used for client to connect to server
  * Input Para	: socket address
  * Return Value: if success return client fd, or else -1 is return
  ==================================================================*/
  bool connectTo(SockAddr &addr_);

  /*==================================================================
  * Function	: CommBase.sendData
  * Description	: send data
  * Input Para	: data_--data buf pointer
  * Input Para	: len_--data buf length
  * Return Value: success send length
  ==================================================================*/
  virtual int sendData(void *data_, 
      uint32_t len_);

  /*==================================================================
  * Function	: CommBase.sendData
  * Description	: send data
  * Input Para	: data_--data buf pointer
  * Input Para	: len_--data buf length
  * Input Para	: addr_--address
  * Return Value: success send length
  ==================================================================*/
  virtual int sendData(void *data_, 
      uint32_t len_, 
      SockAddr &addr_);

  /*==================================================================
  * Function	: CommBase.recvData
  * Description	: recv data
  * Input Para	: data_--data buf pointer
  * Input Para	: len_--data buf length
  * Return Value: success recv length
  ==================================================================*/
  virtual int recvData(void *data_, 
      uint32_t len_);

  /*==================================================================
  * Function	: CommBase.recvData
  * Description	: recv data
  * Input Para	: data_--data buf pointer
  * Input Para	: len_--data buf length
  * Input Para	: addr_--address
  * Return Value: success recv length
  ==================================================================*/
  virtual int recvData(void *data_, 
      uint32_t len_,
      SockAddr &addr_);

  /*==================================================================
  * Function	: CommBase.enableSockReuse
  * Description	: enable socket address reuse
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool enableSockReuse();

  /*==================================================================
  * Function	: CommBase.setSendBufSize
  * Description	: set send buffer size
  * Input Para	: size_--buffer size
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool setSendBufSize(uint32_t size_);

  /*==================================================================
  * Function	: CommBase.setRecvBufSize
  * Description	: set recv buffer size
  * Input Para	: size_--buffer size
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool setRecvBufSize(uint32_t size_);

  /*==================================================================
  * Function	: CommBase.getSendBufSize
  * Description	: get send buffer size
  * Return Value: if success return buffer size, or else -1 is return
  ==================================================================*/
  int getSendBufSize();

  /*==================================================================
  * Function	: CommBase.getRecvBufSize
  * Description	: get recv buffer size
  * Return Value: if success return buffer size, or else -1 is return
  ==================================================================*/
  int getRecvBufSize();
  
protected:

  //socket fd
  int sockFd;
  
private:

  // Disable copy construction and assignment.
  CommBase(const CommBase&);
  const CommBase &operator = (const CommBase&);
};



/*==================================================================
* Function	: StreamSock 
* Description	: Stream Socket class
==================================================================*/
class StreamSock : public CommBase
{
public:

  /*==================================================================
  * Function	: StreamSock 
  * Description	: construction function
  ==================================================================*/
  StreamSock();

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Destructor function
  ==================================================================*/
  virtual ~StreamSock();

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  bool openSock(SockAddr &addr_);

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  int sendData(void *data_, 
    uint32_t len_);

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  int recvData(void *data_, 
    uint32_t len_);
};

/*==================================================================
* Function	: DgramSock 
* Description	: Dgram Socket class
==================================================================*/
class DgramSock : public CommBase
{
public:

  /*==================================================================
  * Function	: DgramSock 
  * Description	: construction function
  ==================================================================*/
  DgramSock();

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Destructor function
  ==================================================================*/
  virtual ~DgramSock();

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  bool openSock(SockAddr &addr_);

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  int sendData(void *data_, 
    uint32_t len_, 
    SockAddr &addr_);

  /*==================================================================
  * Function	: StreamSock 
  * Description	: Please reference to CommBase
  ==================================================================*/
  int recvData(void *data_, 
    uint32_t len_,
    SockAddr &addr_);
};

#endif /*_SOCKET_BASE_H_*/
