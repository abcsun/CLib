/***************************************************************************************
****************************************************************************************
* FILE		: socket_addr.cc
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


#include "include/log_trace.h"
#include "include/socket_addr.h"

using namespace std;

SockAddr::SockAddr(std::string uri_)
{
  uri = uri_;
  domain = 0;
  type = 0;
  memset(&unixAddr, 0, sizeof(unixAddr));
}
SockAddr::SockAddr(int domain_, int type_)
{
  domain = domain_;
  type = type_;
  memset(&unixAddr, 0, sizeof(unixAddr));
}

SockAddr::~SockAddr()
{
  //do nothing
}

struct sockaddr* SockAddr::getAddr()
{
  if(AF_INET == domain)
  {
    return (struct sockaddr*)&inetAddr;
  }
  else if(AF_UNIX == domain)
  {
    return (struct sockaddr*)&unixAddr;
  }
  else
  {
    logTrace("invalid sockct domain:%d", domain);
    return NULL;
  }
}
int SockAddr::getDomain()
{
  return domain;
}
int SockAddr::getType()
{
  return type;
}

socklen_t SockAddr::getSockLen()
{
  if(AF_INET == domain)
  {
    return sizeof(inetAddr);
  }
  else if(AF_UNIX == domain)
  {
    return sizeof(unixAddr);
  }
  else
  {
    logTrace("invalid sockct domain:%d", domain);
    return 0;
  }
}


/*
"stream://127.0.0.1:8888"
"dgram://127.0.0.1:8888"
"stream://string"
"dgram://string"
*/
bool SockAddr::parseUri()
{
  string::size_type pos = uri.find ("://");
  if(string::npos == pos) 
  {
    logTrace("invalid uri:%s", uri.c_str());
    return false;
  }
  
  string protocol = uri.substr (0, pos);
  string addrStr = uri.substr (pos + 3);
  if(protocol.empty () || addrStr.empty ()) 
  {
    logTrace("invalid uri:%s", uri.c_str());
    return false;
  }

  if("stream" == protocol)
  {
    type = SOCK_STREAM;
  }
  else if("dgram" == protocol)
  {
    type = SOCK_DGRAM;
  }
  else
  {
    logTrace("invalid uri:%s", uri.c_str());
    return false;
  }

  pos = addrStr.find (":");
  if(string::npos == pos) 
  {
    domain = AF_UNIX;
    unixAddr.sun_family = AF_UNIX;
    snprintf(&unixAddr.sun_path[1], unixMaxPath, "%s", addrStr.c_str());
    return true;
  }
  else
  {
    domain = AF_INET;
    inetAddr.sin_family = AF_INET;
    string ip = addrStr.substr (0, pos);
    string port = addrStr.substr (pos + 1);
    if(ip.empty () || port.empty ()) 
    {
      logTrace("invalid uri:%s", uri.c_str());
      return false;
    }

    inetAddr.sin_port = htons(atoi(port.c_str()));
    if("*" == ip)
    {
      inetAddr.sin_addr.s_addr = htonl (INADDR_ANY);
    }
    else
    {
      int rc = inet_pton(AF_INET, ip.c_str(), &inetAddr.sin_addr);
      if(rc <= 0)
      {
        logTrace("invalid uri:%s;%m", uri.c_str());
        return false;
      }
    }

    return true;
  }
}

bool SockAddr::toStr(string &str_)
{
  if("" != uri)
  {
    str_ = uri;
    return true;
  }
  
  if(AF_INET == domain)
  {
    char ipStr[INET_ADDRSTRLEN] = {0};
    if(NULL == inet_ntop(AF_INET, &inetAddr.sin_addr, ipStr, INET_ADDRSTRLEN))
    {
      logTrace("invalid sin_addr;%m");
      return false;
    }

    char tmp[256] = {0};
    if(SOCK_STREAM == type)
    {
      snprintf(tmp, sizeof(tmp), "stream://%s:%d", ipStr, ntohs(inetAddr.sin_port));
    }
    else if(SOCK_DGRAM == type)
    {
      snprintf(tmp, sizeof(tmp), "dgram://%s:%d", ipStr, ntohs(inetAddr.sin_port));
    }
    else
    {
      logTrace("invalid type:%d", type);
      return false;
    }
    
    str_ = tmp;
    uri = tmp;

    return true;
  }
  else if(AF_UNIX == domain)
  {
    char tmp[256] = {0};
    if(SOCK_STREAM == type)
    {
      snprintf(tmp, sizeof(tmp), "stream://%s", &unixAddr.sun_path[1]);
    }
    else if(SOCK_DGRAM == type)
    {
      snprintf(tmp, sizeof(tmp), "dgram://%s", &unixAddr.sun_path[1]);
    }
    else
    {
      logTrace("invalid type:%d", type);
      return false;
    }

    str_ = tmp;
    uri = tmp;

    return true;
  }
  else
  {
    logTrace("invalid sockct domain:%d", domain);
    return false;
  }
}
