/***************************************************************************************
****************************************************************************************
* FILE		: socket.h
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


#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sock_ev.h"

class EventLoop;
class CommBase;
class SockAddr;


/*==================================================================
* Function	: Socket
* Description	: Socket used for app, adapter CommBase
==================================================================*/
class Socket
{
public:

  /*==================================================================
  * Function	: Socket.create
  * Description	: static function used for create Socket,
  * Return Value: Socket pointer
  ==================================================================*/
  static Socket* create();

  /*==================================================================
  * Function	: Socket.destroy
  * Description	: static function used for destroy Socket created by Socket.create
  * Input Para	: sock_--socket pointer
  ==================================================================*/
  static void destroy(Socket* &sock_);

  /*==================================================================
  * Function	: Socket.getFd
  * Description	: get socket fd
  * Return Value: socket fd,if fd is not avaliable -1 is return
  ==================================================================*/
  int getFd();

  /*==================================================================
  * Function	: Socket.open
  * Description	: open socket in server of unconnection-mode
  * Input Para	: socket address uri
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool open(const char *uri_);

  /*==================================================================
  * Function	: Socket.connect
  * Description	: connection-mode client connect to server
  * Input Para	: socket address uri
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool connect(const char *uri_);

  /*==================================================================
  * Function	: Socket.accept
  * Description	: connection-mode server accept client connection
  * Input Para	: sock_--stand for client socket pointer
  * Return Value: if success return true, or else false is return
  ==================================================================*/
  bool accept(Socket *sock_);

  /*==================================================================
  * Function	: Socket.send
  * Description	: send data
  * Input Para	: data_--data pointer
  * Input Para	: len_--data length
  * Return Value: the number of characters sent
  ==================================================================*/
  int send(void *data_, 
      uint32_t len_);

  /*==================================================================
  * Function	: Socket.send
  * Description	: send data
  * Input Para	: data_--data pointer
  * Input Para	: len_--data length
  * Input Para	: to_--the address of the target
  * Return Value: the number of characters sent
  ==================================================================*/
  int send(void *data_, 
      uint32_t len_, 
      const char *to_);

  /*==================================================================
  * Function	: Socket.recv
  * Description	: recv data
  * Input Para	: data_--data pointer
  * Input Para	: len_--data length
  * Return Value: the number of characters received
  ==================================================================*/
  int recv(void *data_, 
      uint32_t len_);

  /*==================================================================
  * Function	: Socket.recv
  * Description	: recv data
  * Input Para	: data_--data pointer
  * Input Para	: len_--data length
  * Input Para	: from_--the address of the source
  * Return Value: the number of characters received
  ==================================================================*/
  int recv(void *data_, 
      uint32_t len_,
      char *from_);

  /*==================================================================
  * Function	: Socket.getEvt
  * Description	: get event type
  * Return Value: already register event
  ==================================================================*/
  EventType getEvt();

  /*==================================================================
  * Function	: Socket.processEvent
  * Description	: process Event
  * Input Para	: evt_--event
  ==================================================================*/
  void processEvent(EventType evt_);

  /*==================================================================
  * Function	: Socket.setCallBack
  * Description	: set calback function
  ==================================================================*/
  void setCallBack(EventLoop *loop_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_);

  /*==================================================================
  * Function	: Socket.clearCallBack
  * Description	: clear calback function
  ==================================================================*/
  void clearCallBack(EventType evt_);

private:

  EventType   evt;
  CommBase   *comm;
  SockAddr   *addr;
  EventLoop  *loop;
  EvCallBack  rdCb;
  EvCallBack  wrCb;
  void       *rdArg;
  void       *wrArg;
  
  // Disable copy construction and assignment.
  Socket();
  virtual ~Socket();
  Socket(const Socket&);
  const Socket &operator = (const Socket&);
};

#endif /*_SOCKET_H_*/
