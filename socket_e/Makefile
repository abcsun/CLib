all: log_trace socket_addr socket_base event_loop socket event_dispatcher  

log_trace:
	libtool --mode=compile g++ -c log_trace.cpp
	libtool --mode=link g++ -o liblog_trace.la log_trace.lo -rpath /usr/lib
socket_addr:
	libtool --mode=compile g++ -c socket_addr.cpp
	libtool --mode=link g++ -o libsocket_addr.la socket_addr.lo -rpath /usr/lib

socket_base:
	libtool --mode=compile g++ -c socket_base.cpp
	libtool --mode=link g++ -o libsocket_base.la socket_base.lo -rpath /usr/lib

sock_ev:
	libtool --mode=compile g++ -c sock_ev.cpp
	libtool --mode=link g++ -o libsock_ev.la sock_ev.lo -rpath /usr/lib

event_loop:
	libtool --mode=compile g++ -c event_loop.cpp
	libtool --mode=link g++ -o libevent_loop.la event_loop.lo -rpath /usr/lib

socket:
	libtool --mode=compile g++ -c socket.cpp
	libtool --mode=link g++ -o libsocket.la socket.lo -rpath /usr/lib

event_dispatcher:
	libtool --mode=compile g++ -c event_dispatcher.cpp
	libtool --mode=link g++ -o libevent_dispatcher.la event_dispatcher.lo -rpath /usr/lib

test: server client 

client:
	libtool --mode=compile g++ -c client_test.cpp
	libtool --mode=link g++ -o libclient_test.la client_test.lo
	libtool --mode=link g++ -o client_test libclient_test.la libsocket.la libsocket.la liblog_trace.la libevent_loop.la libevent_dispatcher.la libsocket_addr.la libsocket_base.la 



server:
	libtool --mode=compile g++ -c server_test.cpp
	libtool --mode=link g++ -o libserver_test.la server_test.lo
	libtool --mode=link g++ -o server_test libserver_test.la libsocket.la liblog_trace.la libevent_loop.la libevent_dispatcher.la libsocket_addr.la libsocket_base.la 

clean:
	rm -rf *.o *.lo *.la *~ 
	rm -rf .libs/ include/*~
