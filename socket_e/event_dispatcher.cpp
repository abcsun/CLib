#include "include/socket.h"
#include "include/event_dispatcher.h"
#include "include/sock_ev.h"
#include "include/log_trace.h"

#include <errno.h>

EventDispatcher::EventDispatcher()
{
  //do nothing
}

EventDispatcher::~EventDispatcher()
{
  //do nothing
}
SelectDispatcher::SelectDispatcher()
{
  FD_ZERO(&readSet);
  FD_ZERO(&writeSet);
  FD_ZERO(&exceptSet);
}

SelectDispatcher::~SelectDispatcher()
{
}

bool SelectDispatcher::initialize()
{
  return true;
}

bool SelectDispatcher::addEvt(EventLoop *loop_, 
    Socket *sock_,
    EvCallBack cb_,
    EventType evt_,
    void *arg_)
{
  if(evt_ & evRead)
  {
    FD_SET(sock_->getFd(), &readSet);
  }
  
  if(evt_ & evWrite)
  {
    FD_SET(sock_->getFd(), &writeSet);
  }
  
  if(evt_ & evError)
  {
    FD_SET(sock_->getFd(), &exceptSet);
  }
  
  sock_->setCallBack(loop_, cb_, evt_, arg_);
  
  return true;
}

bool SelectDispatcher::removeEvt(Socket *sock_,
    EventType evt_)
{
  sock_->clearCallBack(evt_);
  
  if(evt_ & evRead)
  {
    FD_CLR(sock_->getFd(), &readSet);
  }
  
  if(evt_ & evWrite)
  {
    FD_CLR(sock_->getFd(), &writeSet);
  }
  
  if(evt_ & evError)
  {
    FD_CLR(sock_->getFd(), &exceptSet);
  }

  return true;
}

int SelectDispatcher::listen(SockMapT &sockMap_, 
    int timeout_)
{
  timeval  timeout;
  timeout.tv_sec = timeout_/1000;
  timeout.tv_usec = timeout_ % 1000 * 1000;

  fd_set tmpReadSet = readSet;
  fd_set tmpWriteSet = writeSet;

  int maxFd = sockMap_.rbegin()->first;
  int num = select(maxFd + 1, &tmpReadSet, &tmpWriteSet, NULL,
                    (timeout_ >=0) ? (&timeout):NULL);
  if (num <= 0)
  {
    logTrace("num=%d;size:%d;%m", num, sockMap_.size());
    return num;
  }

  SockMapT::iterator it = sockMap_.begin();
  while (it != sockMap_.end())
  {
    SockMapT::iterator tmpIt = it++;
    if(FD_ISSET(tmpIt->first, &tmpReadSet))
    {
      tmpIt->second->processEvent(evRead);
    }
    
    if(FD_ISSET(tmpIt->first, &tmpWriteSet))
    {
      tmpIt->second->processEvent(evWrite);
    }
  }
  
  return num;
}

PollDispatcher::PollDispatcher()
{
  //do nothing
}

PollDispatcher::~PollDispatcher()
{
  pollFds.clear();
}

PollFdVecT::iterator PollDispatcher::getPos(int fd_)
{
  PollFdVecT::iterator it;
  for(it = pollFds.begin(); it != pollFds.end(); it++)
  {
    if(it->fd == fd_)
    {
      return it;
    }
  }

  return it;
}
bool PollDispatcher::initialize()
{
  return true;
}

bool PollDispatcher::addEvt(EventLoop *loop_, 
    Socket *sock_,
    EvCallBack cb_,
    EventType evt_,
    void *arg_)
{
  struct pollfd pfd;
  memset(&pfd, 0, sizeof(pfd));

  pfd.fd = sock_->getFd();
  PollFdVecT::iterator it = getPos(pfd.fd);
  if(it == pollFds.end())
  {
    if(evt_ & evRead)
    {
      pfd.events |= POLLIN;
    }
    
    if(evt_ & evWrite)
    {
      pfd.events |= POLLOUT;
    }
    pollFds.push_back(pfd);
  }
  else
  {
    if(evt_ & evRead)
    {
      it->events |= POLLIN;
    }
    
    if(evt_ & evWrite)
    {
      it->events |= POLLOUT;
    }
  }

  sock_->setCallBack(loop_, cb_, evt_, arg_);
  
  return true;
}

bool PollDispatcher::removeEvt(Socket *sock_,
    EventType evt_)
{
  sock_->clearCallBack(evt_);
  
  PollFdVecT::iterator it = getPos(sock_->getFd());
  if(it == pollFds.end())
  {
    logTrace("can't find this in poll event vector");
    return false;
  }
  else
  {
    EventType evt = ~evt_&sock_->getEvt();
    if(0 == evt)
    {
      pollFds.erase(it);
      return true;
    }
  
    if(evt & evRead)
    {
      it->events |= POLLIN;
    }
    
    if(evt & evWrite)
    {
      it->events |= POLLOUT;
    }
  }
  
  return true;
}

int PollDispatcher::listen(SockMapT &sockMap_, 
    int timeout)
{
  int num = poll(&pollFds[0], pollFds.size(), timeout);
  if(num <= 0)
  {
    //logTrace("num=%d;size:%d;%m", num, sockMap_.size());
    return num;
  }

  uint32_t index = 0;
  for(index = 0; index < pollFds.size(); index++)
  {
    struct pollfd pfd = pollFds[index];
    SockMapT::iterator mapIt = sockMap_.find(pfd.fd);
    if(mapIt == sockMap_.end())
    {
      logTrace("can't find this fd in map");
      continue;
    }
  
    if(pfd.revents & POLLIN)
    {
      mapIt->second->processEvent(evRead);
    }
    
    if(pfd.revents & POLLOUT)
    {
      mapIt->second->processEvent(evWrite);
    }
  }
  
  return num;
}

EpollDispatcher::EpollDispatcher()
{
  epollFd = -1;
}

/// 析构函数
EpollDispatcher::~EpollDispatcher()
{
  close(epollFd);
}
bool EpollDispatcher::initialize()
{
  epollFd = epoll_create(256);
  if(-1 == epollFd)
  {
    logTrace("epoll_create failed;%m");
    return false;
  }
  return true;
}

bool EpollDispatcher::addEvt(EventLoop *loop_, 
    Socket *sock_,
    EvCallBack cb_,
    EventType evt_,
    void *arg_)
{
  epoll_event epEvt;
  memset(&epEvt, 0, sizeof(epEvt));
  epEvt.data.fd = sock_->getFd();
  
  EventType evt = evt_|sock_->getEvt();
  if(evt & evRead)
  {
    epEvt.events |= EPOLLIN;
  }
  
  if (evt & evWrite)
  {
    epEvt.events |= EPOLLOUT;
  }

  if(epoll_ctl(epollFd, EPOLL_CTL_MOD, sock_->getFd(), &epEvt) != 0)
  {
    if(errno == ENOENT)
    {
      if(epoll_ctl(epollFd, EPOLL_CTL_ADD, sock_->getFd(), &epEvt) != 0)
      {
        logTrace("epoll_ctl(EPOLL_CTL_ADD) failed,%m");
        return false;
      }
    }
    else
    {
      logTrace("epoll_ctl(EPOLL_CTL_MOD) failed,%m");
      return false;
    }
  }

  sock_->setCallBack(loop_, cb_, evt_, arg_);
  
  return true;
}

bool EpollDispatcher::removeEvt(Socket *sock_,
    EventType evt_)
{
  sock_->clearCallBack(evt_);
  epoll_event epEvt;
  if(epoll_ctl(epollFd, EPOLL_CTL_DEL, sock_->getFd(), &epEvt) != 0)
  {
    logTrace("epoll_ctl(EPOLL_CTL_DEL) failed,%m");
    return false;
  }

  EventType evt = ~evt_&sock_->getEvt();
  if(0 == evt)
  {
    return true;
  }
  
  memset(&epEvt, 0, sizeof(epEvt));
  epEvt.data.fd = sock_->getFd();
  
  if(evt & evRead)
  {
    epEvt.events |= EPOLLIN;
  }
  
  if (evt & evWrite)
  {
    epEvt.events |= EPOLLOUT;
  }

  if(epoll_ctl(epollFd, EPOLL_CTL_ADD, sock_->getFd(), &epEvt) != 0)
  {
    logTrace("epoll_ctl(EPOLL_CTL_ADD) failed,%m");
    return false;
  }

  return true;
}

int EpollDispatcher::listen(SockMapT &sockMap_, 
    int timeout)
{
  epoll_event epEvts[256];
  int num = epoll_wait(epollFd, epEvts, 256, timeout);

  if(num <= 0)
  {
    logTrace("num=%d;size:%d;%m", num, sockMap_.size());
    return num;
  }

  for(int idx = 0; idx < num; ++idx)
  {
    int fd = epEvts[idx].data.fd;
    SockMapT::iterator tmpIt = sockMap_.find(fd);
    if(tmpIt == sockMap_.end())
    {
      logTrace("can't find this fd in map");
      continue;
    }
  
    if(epEvts[idx].events & EPOLLIN)
    {
      tmpIt->second->processEvent(evRead);
    }
    
    if(epEvts[idx].events & EPOLLOUT)
    {
      tmpIt->second->processEvent(evWrite);
    }
  }
  
  return num;
}
