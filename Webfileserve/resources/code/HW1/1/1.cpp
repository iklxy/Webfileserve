#include<iostream>
using namespace std;
//定义了一个名为Node的结构体，用于维护顺序表以及顺序表的长度和可存储的数据规模
struct Node
{
    int* data;//指向该顺序表首地址的指针
    int length;//顺序表的长度
    int size;//顺序表初始化可存储的数据规模
    Node(int initSize)//Node的构造函数 initsize指定顺序表可存储的数据规模
    {
        data = new int[initSize];//在堆区开辟一段内存 用来作为顺序表 让data指向它
        length = 0;
        size = initSize;
    }
    ~Node()//Node的析构函数，在Node类型的对象结束时，释放堆区的内存
    {
        if(data!=nullptr)
        {
            delete[]data;
            data=nullptr;
        }
    }
};
int main()
{
    /*算法思路：双数组解决 开辟一个和数组data一样大小的数组temp，用来存储data中每一位数据
    偏移之后的位置，然后将temp数组中的内容按相同位置赋值给data数组即可*/
    int n,k;//n size
    cin>>n>>k;
    Node* node = new Node(n);//初始化顺序表
    for(int i=0;i<n;i++)
    {
        cin>>node->data[i];
        node->length++;//获取顺序表的长度
    }
    int* temp= new int[node->length];
    for(int i=0;i<node->length;i++)
    {
        temp[(i+k)%node->length]=node->data[i];//data中每个位置的数据偏移k个位置之后存储到temp中对应的位置
    }
    for(int i=0;i<node->length;i++)
    {
        node->data[i]=temp[i];//把temp中的数据拷贝给data
    }
    delete[] temp;
    for(int i=0;i<node->length;i++)
    {
        cout<<node->data[i]<<" ";
    }
    return 0;
}