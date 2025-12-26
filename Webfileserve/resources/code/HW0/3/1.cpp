#include<iostream>
#include<vector>
using namespace std;

void printbee(int n)
{
    vector<long long>malebee;
    vector<long long>femalebee;
    femalebee.push_back(1);
    malebee.push_back(0);
    for(int i=1;i<n;i++)
    {
        malebee.push_back(femalebee[i-1]+malebee[i-1]);
        femalebee.push_back(malebee[i-1]+1);
    }
    cout<<malebee.back()<<" "<<femalebee.back()+malebee.back()<<endl;
}
int main()
{
    int n;
    while(cin>>n && n!=-1)
    {
        printbee(n+1);
    }
    return 0;
}