# 1. 编译器设置
CXX = g++
# 2. 编译选项
CXXFLAGS = -std=c++11 -Wall -g -pthread -I include

# 3. 目标文件名
TARGET = webfileserver

# 4. 源文件列表
SRCS = src/main.cpp \
       src/serve/InetAddress.cpp \
       src/serve/socket.cpp \
       src/serve/webserve.cpp \
       src/http/HttpRequest.cpp \
       src/http/HttpResponse.cpp \
       src/http/HttpConn.cpp \
       src/epoll/Epoll.cpp \
       src/utils/utils.cpp \
       src/log/log.cpp \
       src/timer/heaptimer.cpp \
       src/serve/config.cpp

# 5. 默认目标
all: $(TARGET)

# 6. 生成规则
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# 7. 清理规则
clean:
	rm -f $(TARGET)
	rm -rf log  # 可选：清理日志文件夹