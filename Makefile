CXX = g++
SOURCES = src/client.cpp src/server.cpp

all: client server

client : client.o
	$(CXX) -o bin/client bin/client.o

server : server.o
	$(CXX) -o bin/server bin/server.o

client.o: 
	$(CXX) -std=c++11 -c src/client.cpp -o bin/client.o

server.o:
	$(CXX) -std=c++11 -c src/server.cpp -o bin/server.o

clean:
	rm bin/client bin/servers