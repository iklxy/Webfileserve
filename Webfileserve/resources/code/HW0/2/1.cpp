#include<iostream>
#include<vector>
#include<string>
#include <algorithm>
using namespace std;

void getString(vector<string>&s)
{
    string str;
    while(getline(cin,str))
    {
        str.erase(remove(str.begin(), str.end(), '\r'), str.end());
        if(str=="#") break;
        s.push_back(str);
    }
}

void printString(vector<string>&s)
{
    for(int i=0;i<s.size();i++)
    {
        int sum=0;
        for(int j=0;j<s[i].size();j++)
        {
            if(s[i][j] == '\r') continue; // 跳过回车符
            if(s[i][j]!=' ')
            {
                sum+=(s[i][j]-'A'+1)*(j+1);
            }
        }
        cout<<sum<<endl;
    }
}
int main()
{
    vector<string>s;
    getString(s);
    printString(s);
    return 0;
}