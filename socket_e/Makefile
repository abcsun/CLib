all: log_trace socket_addr socket_base

log_trace:
	libtool --mode=compile g++ -c log_trace.cpp

socket_addr:
	libtool --mode=compile g++ -c socket_addr.cpp

socket_base:
	libtool --mode=compile g++ -c socket_base.cpp

clean:
	rm -rf *.o *.lo *.la *~ 
	rm -rf .libs/ include/*~
