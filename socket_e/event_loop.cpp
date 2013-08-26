#include "include/event_loop.h"
#include "include/sock_ev.h"
#include "include/event_dispatcher.h"
#include "include/socket_base.h"
#include "include/socket.h"
#include "include/log_trace.h"

EventLoop::EventLoop()
{
  dispatcher = NULL;
}
EventLoop::~EventLoop()
{
  if(NULL != dispatcher) delete dispatcher;

  sockMap.clear();
}
bool EventLoop::initialize(LoopType type_)
{
  if(selectLoop == type_)
  {
    dispatcher = new SelectDispatcher();
  }
  else if(pollLoop == type_)
  {
    dispatcher = new PollDispatcher();
  }
  else if(epollLoop == type_)
  {
    dispatcher = new EpollDispatcher();
  }
  else
  {
    logTrace("loop type is not right:%d", type_);
    return false;
  }

  if(NULL == dispatcher)
  {
    logTrace("new EventDispatcher is failed");
    return false;
  }

  if(!dispatcher->initialize())
  {
    logTrace("dispatcher->initialize() is failed");
    return false;
  }

  return true;
}
bool EventLoop::addEvt(Socket *sock_,
    EvCallBack cb_,
    EventType evt_,
    void *arg_)
{
  if((NULL == sock_) || (-1 == sock_->getFd()) || (NULL == cb_))
  {
    logTrace("addEvt input is not valid,pleace check");
    return false;
  }

  if(!dispatcher->addEvt(this, sock_, cb_, evt_, arg_))
  {
    logTrace("addEvt event to loop is failed");
    return false;
  }

  sockMap[sock_->getFd()] = sock_;

  return true;
}
bool EventLoop::removeEvt(Socket *sock_,
    EventType evt_)
{
  if((NULL == sock_) || (-1 == sock_->getFd()))
  {
    logTrace("removeEvt input is not valid,pleace check");
    return false;
  }

  dispatcher->removeEvt(sock_, evt_);

  EventType evt = sock_->getEvt();
  if(0 == evt)
  {
    sockMap.erase(sock_->getFd());
  }

  return true;
}
int EventLoop::listen(int timeout_)
{
  while(1)
  {
    dispatcher->listen(sockMap, timeout_);
  }
}
