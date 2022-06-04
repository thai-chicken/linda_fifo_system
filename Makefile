CXX = g++
SOURCES = src/client.cpp src/server.cpp

all: client server

requests.o:
	$(CXX) -std=c++11 -I . -c src/server/Request.cpp -o bin/request.o

requestcontainer.o: requests.o src/common/TuplePatternMessage.h
	$(CXX) -std=c++11 -I . -c src/server/RequestContainer.cpp -o bin/requestcontainer.o

tuplecontainer.o:  src/common/Tuple.h
	$(CXX) -std=c++11 -I . -c src/server/TupleContainer.cpp -o bin/tuplecontainer.o

tuple.o: src/common/Constants.h src/common/Enums.h src/common/TupleElem.h
	$(CXX) -std=c++11 -I . -c src/common/Tuple.cpp -o bin/tuple.o

tupleelem.o: src/common/Constants.h src/common/Enums.h
	$(CXX) -std=c++11 -I . -c src/common/TupleElem.cpp -o bin/tupleelem.o

tuplemessage.o: src/common/Constants.h src/common/Enums.h src/common/Message.h src/common/Tuple.h
	$(CXX) -std=c++11 -I . -c src/common/TupleMessage.cpp -o bin/tuplemessage.o

tuplepattern.o: src/common/Constants.h src/common/Enums.h src/common/TuplePatternElem.h src/common/Tuple.h
	$(CXX) -std=c++11 -I . -c src/common/TuplePattern.cpp -o bin/tuplepattern.o

tuplepatternelem.o: src/common/Constants.h src/common/Enums.h src/common/TupleElem.h
	$(CXX) -std=c++11 -I . -c src/common/TuplePatternElem.cpp -o bin/tuplepatternelem.o

tuplepatternmessage.o: src/common/Constants.h src/common/Enums.h src/common/TuplePattern.h
	$(CXX) -std=c++11 -I . -c src/common/TuplePatternMessage.cpp -o bin/tuplepatternmessage.o

message.o: src/common/Enums.h src/common/Constants.h src/common/TuplePattern.h src/common/Tuple.h
	$(CXX) -std=c++11 -I . -c src/common/Message.cpp -o bin/message.o

server.o: requestcontainer.o tuplecontainer.o src/common/Message.h 
	$(CXX) -std=c++11 -I . -c src/server/Server.cpp -o bin/server.o

client.o: src/common/Message.h src/common/Constants.h src/common/Enums.h src/common/TupleMessage.h src/common/Message.h src/common/TuplePatternMessage.h src/common/TupleElem.h
	$(CXX) -std=c++11 -I . -c src/client/Client.cpp -o bin/client.o

client: client.o tuplemessage.o tuple.o tupleelem.o tuplepattern.o tuplepatternelem.o message.o src/common/Constants.h tuplepatternmessage.o
	$(CXX) src/main_client.cpp  bin/client.o bin/tupleelem.o bin/tuple.o bin/tuplepattern.o bin/tuplepatternelem.o bin/message.o bin/tuplemessage.o bin/tuplepatternmessage.o `pkg-config --cflags` -std=c++11 -o bin/client -I .

server: server.o message.o tuplemessage.o tuplepatternmessage.o
	$(CXX) src/main_server.cpp bin/server.o bin/request.o bin/requestcontainer.o bin/tuplecontainer.o   bin/message.o bin/tuplemessage.o bin/tuple.o bin/tuplepattern.o bin/tupleelem.o bin/tuplepatternelem.o bin/tuplepatternmessage.o `pkg-config --cflags` -std=c++11 -pthread -o bin/server -I .



clean:
	rm bin/client bin/servers