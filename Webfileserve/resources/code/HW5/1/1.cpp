/*给你一个长度为n的整数数组nums和一个长度为m的整数数组queries，返回一个长度为m的数组answer，其中answer[i]是nums中元素之和小于等于queries[i]的子序列的最大长度。

子序列是由一个数组删除某些元素（也可以不删除）但不改变元素顺序得到的一个数组。
*/
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    int num_len,query_len;
    cin>>num_len>>query_len;
    vector<int> nums(num_len);//存放输入的数组
    vector<int> queries(query_len);//存放查询的数组

    for(int i=0;i<num_len;i++)
    {
        cin>>nums[i];
    }
    for(int i=0;i<query_len;i++)
    {
        cin>>queries[i];
    }

    sort(nums.begin(),nums.end());

    vector<int> sum(num_len+1,0);//前缀和数组
    //因为题目要求的是子序列的最大长度，所以我们需要先对nums进行排序，然后计算前缀和
    for(int i=1;i<=num_len;i++)
    {
        sum[i]=sum[i-1]+nums[i-1];
    }

    vector<int>answer(query_len,0);

    //对于每一个查询，我们都需要遍历前缀和数组，找到最大的满足条件的前缀和的下标
    for(int i=0;i<query_len;i++)
    {
        int temp=queries[i];
        for(int j=1;j<=num_len;j++)
        {
            if(sum[j]>temp)//找到第一个大于temp的前缀和
            {
                answer[i]=j-1;//前一个下标就是我们要找的最大长度
                break;
            }
            else if(sum[j]==temp)//如果前缀和等于temp
            {
                answer[i]=j;//直接返回当前下标
                break;
            }
        }
        if(sum[num_len]<=temp)//如果所有前缀和都小于等于temp
        {
            answer[i]=num_len;//返回数组的长度
        }
    }
        
    for(int i=0;i<query_len;i++)
    {
        if(i!=query_len-1)
          cout<<answer[i]<<" ";
        else
          cout<<answer[i];
    }
    return 0;
}