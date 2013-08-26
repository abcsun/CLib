/***************************************************************************************
****************************************************************************************
* FILE		: socket_addr.h
* Description	: 
*			  
* Copyright (c) 2012 by Liu Yanyun(E-mail:liuyun827@foxmail.com). All Rights Reserved.
*            Without permission, shall not be used for any commercial purpose
* 
* History:
* Version		Name       		Date			Description
   0.1		Liu Yanyun		2012/12/05		Initial Version
   
****************************************************************************************
****************************************************************************************/


#ifndef _SOCKET_ADDR_H_
#define _SOCKET_ADDR_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stddef.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>


/*==================================================================
* class	: SockAddr
* Description	: parse uri address and serialize to string
==================================================================*/
class SockAddr
{
public:

  /*==================================================================
  * Function	: SockAddr.SockAddr
  * Description	: construction function
  * Input Para	: uri address sting
  * Output Para	: null
  * Return Value: 
  ==================================================================*/
  SockAddr(std::string uri_);

  /*==================================================================
  * Function	: SockAddr.SockAddr
  * Description	: construction function
  * Input Para	: domain_ -- address family
  * Input Para	: type_ -- specifies the communication semantics
  * Output Para	: null
  * Return Value: 
  ==================================================================*/
  SockAddr(int domain_, int type_);

  /*==================================================================
  * Function	: SockAddr.~SockAddr
  * Description	: Destructor function
  * Input Para	: 
  * Output Para	: 
  * Return Value: 
  ==================================================================*/
  virtual ~SockAddr();

  /*==================================================================
  * Function	: SockAddr.parseUri
  * Description	: parse uri to socket address
  * Input Para	: void
  * Output Para	: 
  * Return Value: if success return true,or else false return
  ==================================================================*/
  bool parseUri();

  /*==================================================================
  * Function	: SockAddr.toStr
  * Description	: convert socket address to uri
  * Input Para	: store uri buf
  * Output Para	: uri string
  * Return Value: if success return true,or else false return
  ==================================================================*/
  bool toStr(std::string &str_);

  /*==================================================================
  * Function	: SockAddr.getDomain
  * Description	: get socket domain
  * Input Para	: void
  * Output Para	: 
  * Return Value: address famliy
  ==================================================================*/
  int getDomain();

  /*==================================================================
  * Function	: SockAddr.getType
  * Description	: get communication type
  * Input Para	: void
  * Output Para	: 
  * Return Value: communication type
  ==================================================================*/
  int getType();

  /*==================================================================
  * Function	: SockAddr.getSockLen
  * Description	: get socket addrss length
  * Input Para	: void
  * Output Para	: 
  * Return Value: socket addrss length
  ==================================================================*/
  socklen_t getSockLen();

  /*==================================================================
  * Function	: SockAddr.getAddr
  * Description	: get socket addrss
  * Input Para	: void
  * Output Para	: 
  * Return Value: socket addrss pointer
  ==================================================================*/
  struct sockaddr* getAddr();

private:

  static const int unixMaxPath = 108 - 1;
  //AF_INET; AF_UNIX
  int domain;

  //SOCK_STREAM; SOCK_DGRAM
  int type;
  union
  {
    struct sockaddr_in inetAddr;
    struct sockaddr_un unixAddr;
  };

  std::string uri;
};

#endif /*_SOCKET_ADDR_H_*/
