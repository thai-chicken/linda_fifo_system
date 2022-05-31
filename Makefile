CXX = g++
SOURCES = src/client.cpp src/server.cpp

all: requestcontainer.o client server

requests.o:
	$(CXX) -std=c++11 -I . -c src/Request.cpp -o bin/request.o

requestcontainer.o: requests.o
	$(CXX) -std=c++11 -I . -c src/RequestContainer.cpp -o bin/requestcontainer.o

tuplecontainer:  
	$(CXX) -std=c++11 -I . -c src/TupleContainer.cpp -o bin/tuplecontainer.o

client: 
	$(CXX) src/client.cpp src/tuples.pb.cc `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/client 

server: tuplecontainer requestcontainer.o
	$(CXX) src/server.cpp bin/request.o bin/requestcontainer.o bin/tuplecontainer.o src/tuples.pb.cc `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/server

clean:
	rm bin/client bin/servers