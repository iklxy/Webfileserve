/*最长子串*/
#include<iostream>
#include<string>
#include<vector>
using namespace std;

struct Stack
{
    vector<char>data;//存储输入的括号串
    int top;//栈顶指针
    Stack()
    {
        top=-1;
    };
};

//解决括号最长子串函数
void Longest(string input,Stack &s)
{
    int length=input.length();//计算字符串的长度
    int maxlength=0;//最长子串长度
    int startindex=0;//最长子串起始位置
    s.data.push_back(-1);//以-1作为边界
    s.top++;
    for(int i=0;i<length;i++)
    {
        if(input[i]=='(')
        {
            if(s.top + 1 < s.data.size()) 
            {
                s.data[s.top + 1] = i;
            } else 
            {
                s.data.push_back(i);
            }
            s.top++;
        }
        else if(input[i]==')')
        {
            if(s.top!=0)
            {
                s.data.pop_back();
                s.top--;
                int cur_length=i-s.data[s.top];
                if(cur_length>maxlength)
                {
                    maxlength=cur_length;
                    startindex=s.data[s.top]+1;
                }
                else if(cur_length==maxlength)
                {
                    if(s.data[s.top]+1<startindex)
                      startindex=s.data[s.top]+1;
                }
            }
            else
            {
                s.data[s.top]=i;
            }
        }
    }
    cout<<maxlength<<" "<<startindex<<endl;
}
int main()
{
    string input;
    getline(cin,input);
    if (!input.empty() && input.front() == '"' && input.back() == '"') 
    {
        input = input.substr(1, input.size() - 2);
    }
    Stack s;
    Longest(input,s);
}