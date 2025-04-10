CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

all: server

server: main.o server.o request_handler.o log.o input_validation.o utils.o  # <-- Add utils.o here to ensure it's linked
	$(CXX) $(CXXFLAGS) -o server main.o server.o request_handler.o log.o input_validation.o utils.o  # <-- Ensure utils.o is linked

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp

server.o: src/server.cpp src/server.h
	$(CXX) $(CXXFLAGS) -c src/server.cpp

request_handler.o: src/request_handler.cpp src/request_handler.h
	$(CXX) $(CXXFLAGS) -c src/request_handler.cpp

input_validation.o: src/input_validation.cpp src/input_validation.h
	$(CXX) $(CXXFLAGS) -c src/input_validation.cpp

log.o: src/log.cpp src/log.h
	$(CXX) $(CXXFLAGS) -c src/log.cpp

utils.o: src/utils.cpp src/utils.h  # <-- Add this line to compile utils.cpp
	$(CXX) $(CXXFLAGS) -c src/utils.cpp  # <-- Compile utils.cpp

clean:
	rm -f *.o server
