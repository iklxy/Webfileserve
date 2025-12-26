/*给你一个 正 整数 k ，同时给你：
一个大小为 n 的二维整数数组 rowConditions ，其中 rowConditions[i] = [abovei, belowi] 和
一个大小为 m 的二维整数数组 colConditions ，其中 colConditions[i] = [lefti, righti] 。
两个数组里的整数都是 1 到 k 之间的数字。
你需要构造一个 k x k 的矩阵，1 到 k 每个数字需要 恰好出现一次 。剩余的数字都是 0 。
矩阵还需要满足以下条件：
对于所有 0 到 n - 1 之间的下标 i ，数字 abovei 所在的 行 必须在数字 belowi 所在行的上面。
对于所有 0 到 m - 1 之间的下标 i ，数字 lefti 所在的 列 必须在数字 righti 所在列的左边。
返回满足上述要求的矩阵，题目保证若矩阵存在则一定唯一；如果不存在答案，返回一个空的矩阵。*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class Solution
{
    public:
    int node;
    vector<int>answer;//存储拓扑排序排序后的序列
    vector<int>indegree;//记录每一个点的入度 便于后续进行拓扑排序
    vector<vector<int> >graph;//记录图结构
    Solution(int k);
    void buildGraph(vector<pair<int,int> >&condtions);//通过rowCondition和colCondition生成的图结构
    int getanswersize();
    bool topoSort();//拓扑排序函数
};

Solution::Solution(int k)
{
    answer.reserve(k+1);
    graph.reserve(k+1);
    indegree.reserve(k+1);
    node=k;
}

//根据两个condition数组来创建图结构
void Solution::buildGraph(vector<pair<int,int> >&conditions)
{
    for(auto&condition : conditions)
    {
        graph[condition.first].push_back(condition.second);
        indegree[condition.second]++;
    }
}

//对图结构进行拓扑排序
bool Solution::topoSort()
{
    queue<int>q;

    for (int i = 1; i <= node; i++) 
    {
        //遇到入度为0的结点 可以直接入队
        if (indegree[i] == 0)
            q.push(i);
    }

    while(!q.empty())
    {
        //每一次取出队头元素
        int current=q.front();
        q.pop();
        //放入answer数组中，方便后续输出矩阵
        answer.push_back(current);
        //遍历邻居，每次让邻居的入度减1
        for(int next : graph[current])
        {
            indegree[next]--;
            //如果这一次减1之后邻居的入度为0了，则邻居入队
            if(indegree[next]==0)
              q.push(next);
        }
    }
    //返回answer.size() == node 的bool值，若值为true，说明拓扑排序成功
    //若值为false，说明图中有环，拓扑排序失败
    return answer.size() == node;
}

int Solution::getanswersize()
{
    return this->answer.size();
}

int main()
{
    int k,n,m;
    cin>>k>>n>>m;
    vector<pair<int,int> > rowConditions(n);//存储题目输入的行约束条件
    vector<pair<int,int> > colConditions(m);//存储题目输入的列约束条件
    for(int i=0;i<n;i++)
    {
        cin>>rowConditions[i].first>>rowConditions[i].second;
    }
    for(int i=0;i<m;i++)
    {
        cin>>colConditions[i].first>>colConditions[i].second;
    }
    Solution rowgraph(k);
    Solution colgraph(k);
    rowgraph.buildGraph(rowConditions);
    colgraph.buildGraph(colConditions);
    if(!rowgraph.topoSort() || !colgraph.topoSort())
    {
        cout<<-1<<endl;
        return 0;
    }
    else
    {
        //打印矩阵
        vector<vector<int> >matrix(k,vector<int>(k,0));
        vector<int>rowpos(k+1);
        vector<int>colpos(k+1);
        for(int i=0;i<k;i++)
        {
            //记录位置
            rowpos[rowgraph.answer[i]]=i;
            colpos[colgraph.answer[i]]=i;
        }
        for(int i=1;i<=k;i++)
        {
            matrix[rowpos[i]][colpos[i]]=i;
        }
        for(int i=0;i<k;i++)
        {
            for(int j=0;j<k;j++)
            {
                cout<<matrix[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    return 0;
}