/*
* sock_ev.h  
* filled by suncl according to the author's blog (http://blog.csdn.net/gdutliuyun827/article/details/8257186)
*/

#ifndef SOCK_EN_H 
#define SOCK_EN_H 1

#include <map>

class Socket;
class EventLoop;

typedef enum{selectLoop, pollLoop, epollLoop} LoopType;
//typedef enum {evRead, evWrite, evError} EventType;
typedef int EventType;
#define evRead 0x01
#define evWrite 0x02
#define evError 0x03
typedef std::map<int, Socket *> SockMapT;
typedef void(* EvCallBack)(EventLoop *, Socket*, EventType, void *);


#endif
