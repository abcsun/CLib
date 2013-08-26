/***************************************************************************************
****************************************************************************************
* FILE		: event_dispatcher.h
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


#ifndef _EVENT_DISPATCHER_H_
#define _EVENT_DISPATCHER_H_

#include <map>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "sock_ev.h"
#include "event_loop.h"

class Socket;
class EventLoop;

typedef std::vector<struct pollfd> PollFdVecT;



/*==================================================================
* Function	: EventDispatcher
* Description	: event dispatcher base class
==================================================================*/
class EventDispatcher
{
public:

  /*==================================================================
  * Function	: EventDispatcher.EventDispatcher
  * Description	: construction function
  ==================================================================*/
  EventDispatcher();
  
  /*==================================================================
  * Function	: EventDispatcher.~EventDispatcher
  * Description	: Destructor function
  ==================================================================*/
  virtual ~EventDispatcher();
  
  /*==================================================================
  * Function	: EventDispatcher.initialize
  * Description	: implement by subclass
  * Return Value: initialize success return true,or else return false
  ==================================================================*/
  virtual bool initialize() = 0;
  
  /*==================================================================
  * Function	: EventDispatcher.addEvt
  * Description	: add an event and register callback
  * Input Para	: 
  * Output Para	: 
  * Return Value: success return true,or else return false
  ==================================================================*/
  virtual bool addEvt(EventLoop *loop_, 
      Socket *sock_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_) = 0;
      
  /*==================================================================
  * Function	: EventDispatcher.removeEvt
  * Description	: remove an event and un-register callback
  * Input Para	: 
  * Output Para	: 
  * Return Value: success return true,or else return false
  ==================================================================*/
  virtual bool removeEvt(Socket *sock_,
      EventType evt_) = 0;
  /*==================================================================
  * Function	: EventDispatcher.listen
  * Description	: wait for event trigger
  * Input Para	: 
  * Output Para	: 
  * Return Value: 
  ==================================================================*/
  virtual int listen(SockMapT &sockMap_, 
      int timeout_) = 0;
};


/*==================================================================
* Function	: SelectDispatcher
* Description	: use select for wait event
==================================================================*/
class SelectDispatcher : public EventDispatcher
{
public:

  /*==================================================================
  * Function	: SelectDispatcher.SelectDispatcher
  * Description	: construction function
  ==================================================================*/
  SelectDispatcher();

  /*==================================================================
  * Function	: SelectDispatcher.~SelectDispatcher
  * Description	: Destructor function
  ==================================================================*/
  virtual ~SelectDispatcher();

  /*==================================================================
  * Function	: initialize 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool initialize();

  /*==================================================================
  * Function	: addEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool addEvt(EventLoop *loop_, 
      Socket *sock_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_);

  /*==================================================================
  * Function	: removeEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool removeEvt(Socket *sock_,
      EventType evt_);

  /*==================================================================
  * Function	: listen 
  * Description	: Please reference to parent class
  ==================================================================*/
  int listen(SockMapT &sockMap_, 
      int timeout_);
      
private:
  
  fd_set   readSet;
  fd_set   writeSet;
  fd_set   exceptSet;
};


/*==================================================================
* Function	: PollDispatcher
* Description	: use poll for wait event
==================================================================*/
class PollDispatcher : public EventDispatcher
{
public:

  /*==================================================================
  * Function	: PollDispatcher.PollDispatcher
  * Description	: construction function
  ==================================================================*/
  PollDispatcher();

  /*==================================================================
  * Function	: PollDispatcher.~PollDispatcher
  * Description	: Destructor function
  ==================================================================*/
  virtual ~PollDispatcher();

  /*==================================================================
  * Function	: PollDispatcher.getPos
  * Description	: get position in the vector by fd
  * Input Para	: socket fd
  * Return Value: position in the vector
  ==================================================================*/
  PollFdVecT::iterator getPos(int fd_);

  /*==================================================================
  * Function	: initialize 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool initialize();

  /*==================================================================
  * Function	: addEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool addEvt(EventLoop *loop_, 
      Socket *sock_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_);

  /*==================================================================
  * Function	: removeEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool removeEvt(Socket *sock_,
      EventType evt_);

  /*==================================================================
  * Function	: listen 
  * Description	: Please reference to parent class
  ==================================================================*/
  int listen(SockMapT &sockMap_, 
      int timeout_);

private:

    PollFdVecT pollFds;
};


/*==================================================================
* Function	: EpollDispatcher
* Description	: use epoll for wait event
==================================================================*/
class EpollDispatcher : public EventDispatcher
{
public:

  /*==================================================================
  * Function	: EpollDispatcher.EpollDispatcher
  * Description	: construction function
  ==================================================================*/
  EpollDispatcher();

  /*==================================================================
  * Function	: EpollDispatcher.~EpollDispatcher
  * Description	: Destructor function
  ==================================================================*/
  virtual ~EpollDispatcher();

  /*==================================================================
  * Function	: initialize 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool initialize();

  /*==================================================================
  * Function	: addEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool addEvt(EventLoop *loop_, 
      Socket *sock_,
      EvCallBack cb_,
      EventType evt_,
      void *arg_);

  /*==================================================================
  * Function	: removeEvt 
  * Description	: Please reference to parent class
  ==================================================================*/
  bool removeEvt(Socket *sock_,
      EventType evt_);

  /*==================================================================
  * Function	: listen 
  * Description	: Please reference to parent class
  ==================================================================*/
  int listen(SockMapT &sockMap_, 
      int timeout_);
      
private:

    int  epollFd;
};

#endif /*_EVENT_DISPATCHER_H_*/
