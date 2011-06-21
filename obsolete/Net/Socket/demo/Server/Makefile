INCLUDE=-I../../include/ -I/usr/local/include/

LIB=-L/usr/local/lib -L../../src -lThreading -lpthread -lSocket

Server: Server.cpp
	g++ -D_GNU_SOURCE -g Server.cpp -o Server $(INCLUDE) $(LIB)

test: Server
	./Server 15000 
clean:
	rm Server
