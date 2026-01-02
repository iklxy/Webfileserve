# High Performance C++ Web Server

[![Language](https://img.shields.io/badge/language-C%2B%2B11-blue.svg)](https://en.cppreference.com/w/cpp/cpp11)
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)]()
[![Build](https://img.shields.io/badge/build-make-green.svg)]()

Linux 下基于 C++11 实现的轻量级高性能 Web 服务器。
本项目实现了 **Reactor** 高并发模型，利用 **Epoll ET** 模式结合 **非阻塞 IO**，并实现了 **异步日志**、**定时器**、**线程池** 等核心组件。经 Webbench 压测，可支持上万并发连接。

## 核心特性 (Features)

* **高并发模型**：采用 **Epoll Edge Trigger (ET)** 边缘触发模式 + **Non-blocking IO**，使用 **Reactor** 模式处理并发请求。
* **高效线程池**：基于半同步/半反应堆模式实现线程池，利用固定线程数处理计算密集型任务，避免线程频繁创建销毁的开销。
* **零拷贝传输**：利用 **mmap** 内存映射和 **writev** 分散写技术，大幅减少内核态与用户态之间的数据拷贝，提升静态资源传输性能。
* **HTTP 解析**：使用 **有限状态机 (FSM)** 高效解析 HTTP 请求报文，支持 GET 请求及 Keep-Alive 长连接机制。
* **定时器管理**：基于 **小根堆 (Min-Heap)** 实现高性能定时器，以 $O(\log N)$ 复杂度剔除超时非活动连接。
* **异步日志**：实现基于 **阻塞队列** 和 **单例模式** 的异步日志系统，支持按天滚动和日志分级，记录服务器运行状态而不阻塞业务线程。
* **配置灵活**：支持命令行参数解析，可动态配置端口、日志等级、触发模式、超时时间等。（查看config文件夹下的代码）
* 在配置里，依规范，写了数据库的参数，但是代码中没有做连接数据库的处理，有用的参数只有-p 后面接端口，默认的话是8080

## 环境要求 (Requirements)

* **OS**: Linux (Tested on Ubuntu 20.04/22.04)
* **Compiler**: g++ >= 4.8 (需支持 C++11)
* **Tools**: make

## 快速开始 (Getting Started)

### 编译
项目根目录下执行 `make` 即可编译：

```bash
make
```
### 默认端口是8080

### 一些碎碎念
在暑假的时候对Linux有了一点兴趣然后搞了个虚拟机玩了一下。然后自己学了一些计网相关的知识之后，就想自己写一个服务器玩玩。
在本学期开始的时候就开始一点一点的写，断断续续的终于在期末周的几天完成了。（复习累了就写一会）
代码还有很多不足的地方，有一些bug也是询问ai之后才改对的。
resource文件夹中的HTML文件是用Gemini写的，如果有需要想复现这个项目代码的话可以让ai重写一下前端页面。
资源里的photo就是本人高中时期的一些合照，在后面考完期末考了会考虑更新一些资源下载区的文件！
如果可以看到这里了希望点点star ovo

