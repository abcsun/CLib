/***************************************************************************************
****************************************************************************************
* FILE        : server_test.cc
* Description    : 
*             
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
* Without permission, shall not be used for any commercial purpose
* 
* History:
* Version        Name         Date            Description
   0.1        Liu Yanyun        2012/12/03        Initial Version

****************************************************************************************
****************************************************************************************/

#include "include/socket.h"
#include "include/log_trace.h"
#include "include/socket_addr.h"
#include "include/socket_base.h"
#include "include/event_loop.h"
#include <iostream>

using namespace std;

//有可读事件发生(有数据发送过)调用的callback
void readCb(EventLoop *loop_, Socket *sock_, EventType evt_, void *arg_)
{
  char buf[100] = {0};
  int ret = sock_->recv(buf, sizeof(buf));
  if(0 == ret)
  {
    loop_->removeEvt(sock_, evRead);
    Socket::destroy(sock_);
    printf("the other is connection is closed,destroy it\n");
  }

  printf("buf:%s;arg:%s\n", buf, (char*)arg_);
}

/* 
接受新的连接请求的callback；loop_:所在的监听loop，sock_此处是server的Socket，evt_触发的事件， 
arg_注册callback是传递的参数，类似于pthread_create() 
*/
void acceptCb(EventLoop *loop_, Socket *sock_, EventType evt_, void *arg_)
{
  printf("accept callback trigger:%s\n", (char*)arg_);
  Socket *conn = Socket::create();
  sock_->accept(conn);//接受客户端的连接，
  char *test = "test arg sock read";
  loop_->addEvt(conn, readCb, evRead, test);

  char buf[100] = {"hello"};
  conn->send(buf, strlen(buf));
}

int main()
{
  EventLoop loop;//EventLoop用于管理监听的事件，实现了select poll epoll
  loop.initialize(pollLoop);//此处使用poll方式； epollLoop：epoll； selectLoop：select

  Socket *ser = Socket::create();
  bool ret = ser->open("stream://127.0.0.1:369");//在这个地址打开监听端口，可以用;stream://*:8080

  /* 
   添加到事件监听框架中，采取使用callback的形式，下面注册的是read事件，最后的参数是当callback调用的时候回传递过去。 
  */
  char *test = "test arg accept";
  loop.addEvt(ser, acceptCb, evRead, test);

  loop.listen(3000);//此处是循环监听事件的，内部会调用select、poll或epoll；有事件触发，会调用注册的callback

  Socket::destroy(ser);

  return 0;
}
