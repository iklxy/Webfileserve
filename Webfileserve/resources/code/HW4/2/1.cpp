/*假如给你一个社交网络图，请你对每个节点计算符合“六度空间”理论的结点占结点总数的百分比。

说明：由于浮点数精度不同导致结果有误差，请按float计算。*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int BFS(int start,vector<bool>&visited,vector<vector<int> >&graph)
{
    int PathCount=1;//用于记录符合六度空间理论的点的总数
    visited[start]=true;
    queue<pair<int,int> >q;//这里的pair对存储的内容分别是 结点，距离start的距离
    q.push(std::make_pair(start,0));
    while(!q.empty())
    {
        auto temp=q.front();
        int cur=temp.first;
        int depth=temp.second;
        q.pop();
        if(depth==6)//如果当前结点距离start的距离已经为6 则跳过 不执行后续代码
          continue;
        for(int val: graph[cur])//遍历当前结点的邻居
        {
            if(!visited[val])
            {
                visited[val]=true;
                PathCount++;//总数加1
                q.push(std::make_pair(val,depth+1));//入队 且距离start的距离加1
            }
        }
    }
    return PathCount;//返回距离start距离小于等于6的点的总数
}
int main()
{
    int n,m;
    cin>>n>>m;
    vector< vector<int> > graph(n+1);
    vector<bool> visited(n+1,false);
    for(int i=0;i<m;i++)
    {
        int a,b;
        cin>>a>>b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    for(int i=1;i<=n;i++)
    {
        fill(visited.begin(),visited.end(),false);
        int count=BFS(i,visited,graph);
        float ratio=(float)count/n*100;
        printf("%d: %.2f%%\n",i,ratio);
    }
    return 0;
}