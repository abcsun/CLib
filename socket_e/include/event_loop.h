/***************************************************************************************
****************************************************************************************
* FILE		: event_loop.h
* Description	: 
*			  
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
*            Without permission, shall not be used for any commercial purpose
* 
* History:
* Version		Name       		Date			Description
   0.1		Liu Yanyun		2012/12/20		Initial Version
   
****************************************************************************************
****************************************************************************************/


#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <string.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <poll.h>
#include "sock_ev.h"

class Socket;
class EventDispatcher;


/*==================================================================
* Function	: EventLoop
* Description	: event loop,it is adapter for select poll epoll wait event
==================================================================*/
class EventLoop
{
public:

  /*==================================================================
  * Function	: EventLoop.EventLoop
  * Description	: construction function
  ==================================================================*/
  EventLoop();

  /*==================================================================
  * Function	: EventLoop.~EventLoop
  * Description	: Destructor function
  ==================================================================*/
  virtual ~EventLoop();

  /*==================================================================
  * Function	: EventLoop.initialize
  * Description	: initialize the loop
  * Input Para	: type_--dispatcher type
  * Return Value: success return true,or else return false
  ==================================================================*/
  bool initialize(LoopType type_);

  /*==================================================================
  * Function	: EventLoop.addEvt
  * Description	: add an event and register callback
  * Input Para	: 
  * Output Para	: 
  * Return Value: success return true,or else return false
  ==================================================================*/
  bool addEvt(Socket *sock_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_);

  /*==================================================================
  * Function	: EventLoop.removeEvt
  * Description	: remove an event and un-register callback
  * Input Para	: 
  * Output Para	: 
  * Return Value: success return true,or else return false
  ==================================================================*/
  bool removeEvt(Socket *sock_,
      EventType evt_);

  /*==================================================================
  * Function	: EventLoop.listen
  * Description	: wait for event trigger
  * Input Para	: 
  * Output Para	: 
  * Return Value: 
  ==================================================================*/
  int listen(int timeout_);
      
private:

    EventDispatcher *dispatcher;
    SockMapT  sockMap;
};

#endif /*_EVENT_LOOP_H_*/
