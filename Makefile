IDIR=-I. -I/usr/local/Cellar/boost/1.61.0_1/include/
CC=g++
DEPS =a 

LIBS=-lm -lboost_system 
LDIR=-L/usr/local/Cellar/boost/1.61.0_1/lib

CFLAGS=$(IDIR) $(LDIR)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

TimeClient: timeclient.o
	$(CC)  -o TimeClient main.cpp  timeclient.o $(CFLAGS) $(LIBS)

timeclient.o : timeclient.cpp timeclient.h
	$(CC) -c timeclient.cpp $(CFLAGS) $(LIBS)

#TimeClient:
#	$(CC) -o TimeClient timeclient.cpp $(CFLAGS) $(LIBS)

#TimeClient: timeclient.o  
#	$(CC)  -o TimeClient timeclient.o $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f TimeClient *.o
