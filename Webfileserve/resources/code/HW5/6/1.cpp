#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
using namespace std;

class FreqStack
{
public:
    FreqStack()
    {
        // 这里填写你的代码
    }

    void push(int val)
    {
        // 这里填写你的代码
        // 更新频率表和频率栈
        freq[val]++;
        max_freq = max(max_freq, freq[val]);
        freq_stack[freq[val]].push(val);
    }

    int pop()
    {
        // 这里填写你的代码
        // 从最大频率栈中弹出元素
        int val = freq_stack[max_freq].top();
        freq_stack[max_freq].pop();
        freq[val]--;
        if (freq_stack[max_freq].empty())
        {
            max_freq--;
        }
        return val;
    }
    // 这个表记录频率
    map<int, int> freq;
    // 这个表记录每个频率对应的堆栈
    map<int, stack<int>> freq_stack;
    // 记录当前最大频率
    int max_freq = 0;
};
