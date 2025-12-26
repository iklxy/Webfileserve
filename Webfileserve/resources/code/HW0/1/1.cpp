#include<iostream>
#include<vector>
#include<iomanip>
using namespace std;

int main()
{
    vector<double>money;
    for(int i=0;i<12;i++)
    {
        double x;
        cin>>x;
        money.push_back(x);
    }
    double sum=0;
    for(int i=0;i<12;i++)
    {
        sum+=money[i];
    }
    cout << fixed << setprecision(2);
    cout<<"¥"<<sum/12.00;
    return 0;
}