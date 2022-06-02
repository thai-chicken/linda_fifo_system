CXX = g++
SOURCES = src/client.cpp src/server.cpp

all: client server

requests.o:
	$(CXX) -std=c++11 -I . -c src/server/Request.cpp -o bin/request.o

requestcontainer.o: requests.o
	$(CXX) -std=c++11 -I . -c src/server/RequestContainer.cpp -o bin/requestcontainer.o

tuplecontainer.o:  
	$(CXX) -std=c++11 -I . -c src/server/TupleContainer.cpp -o bin/tuplecontainer.o

server.o: requestcontainer.o tuplecontainer.o src/common/Message.h
	$(CXX) -std=c++11 -I . -c src/server/Server.cpp -o bin/server.o

client.o: src/common/Message.h
	$(CXX) -std=c++11 -I . -c src/client/Client.cpp -o bin/client.o

client: client.o
	$(CXX) src/main_client.cpp src/common/tuples.pb.cc bin/client.o `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/client -I .

server: server.o
	$(CXX) src/main_server.cpp bin/request.o bin/requestcontainer.o bin/tuplecontainer.o  bin/server.o src/common/tuples.pb.cc `pkg-config --cflags --libs protobuf` -std=c++11 -o bin/server -I .

clean:
	rm bin/client bin/servers