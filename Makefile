#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= vances

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: shakespearesearch testclient shakespeareajax PutCGI PutHTML
#all: shakespearesearch testclient 

testclient.o: testclient.cpp fifo.h
	$(CC) -c $(CFLAGS) testclient.cpp

shakespearesearch.o: shakespearesearch.cpp fifo.h
	$(CC) -c $(CFLAGS) shakespearesearch.cpp

shakespeareajax.o: shakespeareajax.cpp fifo.h
	$(CC) -c $(CFLAGS) shakespeareajax.cpp

testclient: testclient.o fifo.o
	$(CC) testclient.o fifo.o -o testclient

shakespearesearch: shakespearesearch.o fifo.o
	$(CC) shakespearesearch.o  fifo.o -o shakespearesearch

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
shakespeareajax: shakespeareajax.o  fifo.o
	$(CC) shakespeareajax.o  fifo.o -o shakespeareajax -L/usr/local/lib -lcgicc

PutCGI: shakespeareajax
	chmod 757 shakespeareajax
	cp shakespeareajax /usr/lib/cgi-bin/$(USER)_shakespeareajax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp shake_stats_ajax.html /var/www/html/class/softdev/$(USER)
	cp shake_stats_ajax.css /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o shake_stats_ajax shakespearesearch testclient
