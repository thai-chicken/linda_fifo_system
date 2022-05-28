CXX = g++
SOURCES = src/client.cpp src/server.cpp

all: client server

# client : client.o
# 	$(CXX) -o bin/client bin/client.o

# server : server.o
# 	$(CXX) -o bin/server bin/server.o

client: 
	$(CXX) src/client.cpp src/tuples.pb.cc `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/client

server:
	$(CXX) src/server.cpp  src/tuples.pb.cc `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/server

clean:
	rm bin/client bin/servers