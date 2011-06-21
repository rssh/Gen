Client: Client.cpp
	g++ Client.cpp -o Client -I../../include/ -L../../src -lSocket

test: Client
	./Client 10.0.1.99 15000 

clean:
	rm -f *.o Client
