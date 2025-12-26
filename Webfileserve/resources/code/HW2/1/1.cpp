/*列出进站*/
#include<iostream>
#include<string>
#include<vector>//用来接受输入的车次串和出站的序列串
using namespace std;

struct Stack
{
    vector<char>data;//作为车站 用于存放已经进站的列车
    int top;//栈顶指针
    int maxsize;
    Stack()
    {
        top=-1;
        maxsize=0;
    };
};

bool Train(Stack &s,vector<char>input,vector<char>output)
{
    int i=0,j=0;
    s.top=-1;
    s.data.clear();
    while(i<input.size())
    {
        if(s.top==-1 || s.data[s.top]!=output[j])//车站为空 或 站顶车次不等于出站车次
        {
            if(i==input.size())
              return false;//车站进车完毕 且车站顶的车次不等于出站车次
            s.data.push_back(input[i]);//进站
            s.top++;
            i++;
        }
        else//车站顶的车次等于出站车次
        {
            s.data.pop_back();//出站
            s.top--;
            j++;
        }
    }
    while(s.top!=-1)
    {
        if(s.data[s.top]!=output[j])
          return false;
        s.data.pop_back();
        s.top--;
        j++;
    }
    return true;
}
int main()
{
    vector<char>input;//代表车次序列
    vector<char>output;//代表出站序列
    Stack s;

    string in_str, out_str;
    getline(cin, in_str); // 读取进站序列
    for(char c : in_str) input.push_back(c);

    int num=input.size();
    s.maxsize=num;

    while(getline(cin, out_str)) // 读取出站序列
    {
        if(out_str=="EOF")
          break;
        output.clear();
        for(char c : out_str) output.push_back(c);
        if(Train(s, input, output))
            cout << "yes" << endl;
        else
            cout << "no" << endl;
    }
}