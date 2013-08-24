
all: install test

test:
	libtool --mode=compile g++ -c vlog_test.cpp
	libtool --mode=link g++ -o vlog_test vlog_test.lo /usr/lib/libvlog.la

install:
	libtool --mode=compile g++ -c vlog.cpp
	libtool --mode=link g++ -o libvlog.la vlog.lo -rpath /usr/lib
	sudo libtool --mode=install install -c libvlog.la /usr/lib

clean:
	rm -rf *.lo *.o *.la .libs *~ 
