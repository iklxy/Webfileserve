#include<iostream>
#include<vector>
using namespace std;
/*定义了一个Series类，类中存储了一个vector容器，用来动态存储一个数字的每一位
理由：常规的for循环解决级数和，在遍历次数过多的时候，其结果会超出long long型的范围
因此考虑使用数组来完成高精度的存储*/
class Series
{
    private:
    //使用vector来存储数字的每一位 series[0]代表个位 由0增加代表由个位变成十位 百位......
    vector<int> series;
    public:
    //Series的构造函数
    Series(){};
    Series(int n)
    {
        if(n==0)
        {
            series.push_back(0);
            return;
        }
        while(n)
        {
            series.push_back(n%10);
            n/=10;
        }
    }
    //overload +
    Series operator+(const Series& other)const
    {
        Series result;
        int carry=0;
        int n=max(series.size(),other.series.size());
        for(int i=0;i<n;i++)
        {
            int digit1=i<series.size()?series[i]:0;
            int digit2=i<other.series.size()?other.series[i]:0;
            int sum=digit1+digit2+carry;
            result.series.push_back(sum%10);
            carry=sum/10;
        }
        if(carry)
        {
            result.series.push_back(carry);
        }
        return result;
    }
    //overload *
    Series operator*(const int n)const
    {
        Series result;
        int carry=0;
        for(int i=0;i<series.size();i++)
        {
            int num=series[i]*n+carry;
            result.series.push_back(num%10);
            carry=num/10;
        }
        if(carry)
        {
            while(carry)
            {
                result.series.push_back(carry%10);
                carry/=10;
            }
        }
        //remove leading zero
        while (result.series.size() > 1 && result.series.back() == 0)
         {
            result.series.pop_back();
        }
        return result;
    }
    //overload <<
    friend ostream& operator<<(ostream& output,Series& s)
    {
        vector<int> result=s.series;
        for(int i = s.series.size() - 1; i >= 0; i--)
        { 
          output << s.series[i];
        }
    return output;
    }
};

int main()
{
    int N,A;
    while(cin>>N>>A)
    {
        if(A==0)
        {
            cout<<0<<endl;
            continue;
        }
        Series result(0);
        Series power(1);
        for(int i=1;i<=N;i++)
        {
            power=power*A;
            result=result+power*(i);
        }
        cout<<result<<endl;
    }
    return 0;
}