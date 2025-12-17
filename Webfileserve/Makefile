CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -I include
TARGET = webfileserver
SRCS = src/main.cpp src/serve/InetAddress.cpp src/serve/socket.cpp src/serve/webserve.cpp src/http/HttpRequest.cpp src/http/HttpResponse.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
