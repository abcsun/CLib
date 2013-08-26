/***************************************************************************************
****************************************************************************************
* FILE        : client_test.cc
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
#include <iostream>

using namespace std;

int main()
{
  Socket *clt = Socket::create();//Socket类封装的socket相关的操作，创建一个Socket
  bool ret = clt->connect("stream://127.0.0.1:369");//向服务器发起连接
  ret = ret;

  char buf[100] = {0};
  clt->recv(buf, sizeof(buf));//发送数据到server端
  cout<<buf<<endl;

  sprintf(buf, "%s", "hello too");
  clt->send(buf, strlen(buf));//接受server发送来的数据

  sleep(20);

  Socket::destroy(clt);//通讯结束销毁创建的Socket

  return 0;
}
