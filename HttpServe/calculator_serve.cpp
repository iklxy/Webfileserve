#include<iostream>
#include<fstream>
#include<cstring>
#include<string>
#include<sstream>
#include<unistd.h>//系统调用
#include<arpa/inet.h>//地址转换
#include<sys/socket.h>//核心socket接口
#include<netinet/in.h>//地址定义
#include<sys/types.h>//系统基本类型支持 例如size_t等数据类型由typedef来定义

const int SIZE=4;

struct Calculator_Message 
{
    int32_t num_count;  // 使用固定大小的类型
    int32_t numbers[100];  // 假设最多100个数字
    char operation;
};

int calculator(int num_count,int num[],char op);

int main(int argc,char* argv[])
{
    int sockfd;//serve socket
    int clientfd;//client socket

    sockaddr_in server_addr;//serve address
    sockaddr_in client_addr;//client address
    socklen_t client_addr_len = sizeof(client_addr);

    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if(sockfd==-1)
    {
        std::cerr<<"Socket creation failed!"<<std::endl;
        return -1;
    }
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr=INADDR_ANY;

    if(bind(sockfd,(sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        std::cerr<<"Bind failed!"<<std::endl;
        close(sockfd);
        return -1;
    }

    if(listen(sockfd,5)==-1)
    {
        std::cerr<<"Listen failed!"<<std::endl;
        close(sockfd);
        return -1;
    }

    std::cout<<"Calculator Server is running on port "<<argv[1]<<"..."<<std::endl;

    for(int i=0;i<5;i++)
    {
        clientfd=accept(sockfd,(sockaddr*)&client_addr,&client_addr_len);
        if(clientfd==-1)
        {
            std::cerr<<"Accept failed!"<<std::endl;
            continue;
        }

        Calculator_Message buffer;
        recv(clientfd,&buffer,sizeof(buffer),0);
    
        int result=calculator(buffer.num_count,buffer.numbers,buffer.operation);
        send(clientfd,&result,sizeof(result),0);
        close(clientfd);
    }
    close(sockfd);
    return 0;
}

int calculator(int num_count,int num[],char op)
{
    int result=num[0];
    for(int i=1;i<num_count;i++)
    {
        if(op=='+')
        {
            result+=num[i];
        }
        else if(op=='-')
        {
            result-=num[i];
        }
        else if(op=='*')
        {
            result*=num[i];
        }
        else
        {
            result/=num[i];
        }
    }
    return result;
}