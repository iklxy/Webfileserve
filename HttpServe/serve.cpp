#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<thread>
#include<sstream>
#include<unistd.h>//系统调用
#include<arpa/inet.h>//地址转换
#include<sys/socket.h>//核心socket接口
#include<netinet/in.h>//地址定义
#include<sys/types.h>//系统基本类型支持 例如size_t等数据类型由typedef来定义

std::string url_decode(const std::string &s)
{
    std::string result;
    result.reserve(s.size());//预分配空间
    for(int i=0;i<s.size();i++)
    {
        if(s[i]=='+')
        {
            result+=' ';
        }
        else if(s[i]=='%'&&i+2<s.size())
        {
            std::string hex=s.substr(i+1,2);
            char ch=static_cast<char>(std::stoi(hex,nullptr,16));
            result+=ch;
            i+=2;
        }
        else
        {
            result+=s[i];
        }
    }
    return result;
}

std::string read_file(const std::string &filepath)
{
    std::ifstream file_in(filepath,std::ios::binary);
    if(!file_in.is_open())return "";
    file_in.seekg(0,std::ios::end);
    size_t filesize=file_in.tellg();
    file_in.seekg(0,std::ios::beg);
    
    std::string body(filesize, '\0');
    file_in.read(&body[0],filesize);
    file_in.close();
    return body;
}

std::string build_Http_response(const std::string &body,const std::string &message)
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << 200 << " " << message << "\r\n";
    oss << "Content-Type: text/html; charset=utf-8\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "\r\n" << body;
    return oss.str();
}
void handle_clinet(int clientfd,sockaddr_in client_addr)
{
    char buffer[1024];
    ssize_t n=recv(clientfd,buffer,sizeof(buffer)-1,0);
    if(n>0)
    {
        buffer[n]='\0';
    }

    std::string request(buffer);

    std::istringstream iss(request);
    std::string method,path,version;
    iss>>method>>path>>version;

    if(path.empty())path = "/";
    try{path = url_decode(path);}
    catch(const std::exception& e){
        path = "/";
    }
    if(path=="/")
    {
        path="/index.html";
    }
    std::string filepath="www"+path;

    std::string body;
    body=read_file(filepath);
    std::string response=build_Http_response(body,"OK");

    send(clientfd, response.c_str(), response.size(), 0);
    close(clientfd);
}
int main(){
    //1.创建套接字
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        std::cerr<<"Socket creation failed!"<<std::endl;
        return -1;
    }
    //2.绑定地址信息
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=INADDR_ANY;
    addr.sin_port=htons(8080);//htons函数用于将端口序号转换为大端序
    if(bind(sockfd,(sockaddr*)&addr,sizeof(addr))==-1)
    {
        std::cerr<<"Bind failed!"<<std::endl;
        close(sockfd);
        return -1;
    }
    if (listen(sockfd, 5) < 0) 
    {
        perror("listen");
        return -1;
    }
    std::cout<<"HTTP Server is running on port 8080..."<<std::endl;
    //3.接收数据
    sockaddr_in client;
    socklen_t clientlen=sizeof(client);
    while(true)
    {
        int clientfd=accept(sockfd,(sockaddr*)&client,&clientlen);
        if(clientfd==-1)
        {
            std::cerr<<"Accept failed!"<<std::endl;
            continue;
        }
        if (clientfd >= 0)
            std::thread(handle_clinet, clientfd, client).detach();
    }
    close(sockfd);
    return 0;
}