#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
class Solution
{
public:
    std::unordered_map<std::string, int> target_position;//存储目标位置
    std::vector<std::string> now_position;//存储当前的位置
    int solve(std::vector<std::vector<std::string>> &old_chart, std::vector<std::vector<std::string>> &new_chart)
    {
        // 展开old_chart到一维数组 记录当前位置
        for (int i = 0; i < old_chart.size(); i++)
        {
            for (int j = 0; j < old_chart[0].size(); j++)
            {
                now_position.push_back(old_chart[i][j]);
            }
        }
        int position = 0;
        // 展开new_chart到哈希表 记录目标位置
        for (int i = 0; i < new_chart.size(); i++)
        {
            for (int j = 0; j < new_chart[0].size(); j++)
            {
                target_position[new_chart[i][j]] = position++;
            }
        }

        int res = 0;
        for (int i = 0; i < now_position.size(); i++)
        {
            while (true)
            {
                // 当前元素在目标位置上 则跳出循环
                if (target_position[now_position[i]] == i)
                    break;
                else
                {
                    // 否则交换到目标位置上
                    std::swap(now_position[i], now_position[target_position[now_position[i]]]);
                    res++;
                }
            }
        }
        return res;
    }
};