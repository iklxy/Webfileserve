/*布尔表达式*/
#include<iostream>
#include<vector>
#include<string>
using namespace std;
//全局变量计数器
int count_sum=0;
//字符栈
struct Stack_num
{
    vector<char>data;
    int top;
    Stack_num()
    {
        top=-1;
    };
};

//符号栈 符号优先级 !>&>|
struct Stack_op
{
    vector<char>data;
    int top;
    Stack_op()
    {
        top=-1;
    };
};

void Bollexpression(string input,Stack_num &s_num,Stack_op &s_op)
{
    //先清空两个栈
    s_num.top=-1;
    s_num.data.clear();
    s_op.top=-1;
    s_op.data.clear();
    for(char ch: input)
    {
        if(ch==' ')continue;
        if(ch=='F'||ch=='V')
        {
            s_num.data.push_back(ch);
            s_num.top++;
        }
        //因为左括号和!的优先级最高 所以直接入栈即可
        if(ch=='(')
        {
            s_op.data.push_back(ch);
            s_op.top++;
        }
        if(ch=='!')
        {
            s_op.data.push_back(ch);
            s_op.top++;
        }
        //处理右括号
        if(ch==')')
        {
            while(s_op.top!=-1&&s_op.data[s_op.top]!='(')
            {
                char op=s_op.data[s_op.top];//拿到栈顶的运算符
                s_op.data.pop_back();
                s_op.top--;
                if(op=='!')
                {
                    char num=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num=='F')s_num.data.push_back('V');
                    else s_num.data.push_back('F');
                    s_num.top++;
                }
                else if(op=='&')
                {
                    char num1=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    char num2=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num1=='V'&&num2=='V')s_num.data.push_back('V');
                    else s_num.data.push_back('F');
                    s_num.top++;
                }
                else if(op=='|')
                {
                    char num1=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    char num2=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num1=='F'&&num2=='F')s_num.data.push_back('F');
                    else s_num.data.push_back('V');
                    s_num.top++;
                }
            }
            //把左括号弹出
            if(s_op.top!=-1&&s_op.data[s_op.top]=='(')
            {
                s_op.data.pop_back();
                s_op.top--;
            }
        }
        else if(ch=='&'||ch=='|')
        {
            while(s_op.top!=-1&&(s_op.data[s_op.top]=='!'||(s_op.data[s_op.top]=='&'&&ch=='|')))
            {
                char op=s_op.data[s_op.top];//拿到栈顶的运算符
                s_op.data.pop_back();
                s_op.top--;
                if(op=='!')
                {
                    char num=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num=='F')s_num.data.push_back('V');
                    else s_num.data.push_back('F');
                    s_num.top++;
                }
                else if(op=='&')
                {
                    char num1=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    char num2=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num1=='V'&&num2=='V')s_num.data.push_back('V');
                    else s_num.data.push_back('F');
                    s_num.top++;
                }
                else if(op=='|')
                {
                    char num1=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    char num2=s_num.data[s_num.top];
                    s_num.data.pop_back();
                    s_num.top--;
                    if(num1=='F'&&num2=='F')s_num.data.push_back('F');
                    else s_num.data.push_back('V');
                    s_num.top++;
                }
            }
            //把当前符号入栈
            s_op.data.push_back(ch);
            s_op.top++;
        }
    }
    //把栈中剩余的符号依次出栈
    while(s_op.top!=-1)
    {
        char op=s_op.data[s_op.top];//拿到栈顶的运算符
        s_op.data.pop_back();
        s_op.top--;
        if(op=='!')
        {
            char num=s_num.data[s_num.top];
            s_num.data.pop_back();
            s_num.top--;
            if(num=='F')s_num.data.push_back('V');
            else s_num.data.push_back('F');
            s_num.top++;
        }
        else if(op=='&')
        {
            char num1=s_num.data[s_num.top];
            s_num.data.pop_back();
            s_num.top--;
            char num2=s_num.data[s_num.top];
            s_num.data.pop_back();
            s_num.top--;
            if(num1=='V'&&num2=='V')s_num.data.push_back('V');
            else s_num.data.push_back('F');
            s_num.top++;
        }
        else if(op=='|')
        {
            char num1=s_num.data[s_num.top];
            s_num.data.pop_back();
            s_num.top--;
            char num2=s_num.data[s_num.top];
            s_num.data.pop_back();
            s_num.top--;
            if(num1=='F'&&num2=='F')s_num.data.push_back('F');
            else s_num.data.push_back('V');
            s_num.top++;
        }
    }
    cout<<"Expression "<<count_sum<<": "<<s_num.data[s_num.top]<<endl;
}

int main()
{
    string input;
    Stack_num s_num;
    Stack_op s_op;
    while(getline(cin,input))
    {
        count_sum++;
        Bollexpression(input,s_num,s_op);
    }
}