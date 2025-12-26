/*本题给定一个无向图，用dfs和bfs找出图的所有连通分量。
所有顶点用0到n-1表示，搜索时总是从编号最小的顶点出发。使用邻接矩阵存储，或者邻接表（使用邻接表时需要使用尾插法）。*/
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

vector<int> temppath;

//BFS
void BFS(int start,vector<bool>&visited,vector<vector<int> >&paragraph)
{
    vector<int>path;//记录BFS生成的路径
    visited[start]=true;//将当前访问结点标记为已访问
    queue<int> q;//使用队列进行BFS
    q.push(start);
    while(!q.empty())
    {
        int cur=q.front();//每一次取出队列头的元素
        path.push_back(cur);
        q.pop();
        for(int val : paragraph[cur])//遍历队列头元素的邻居
        {
            if(!visited[val])//如果邻居未被访问过 则入队
            {
                visited[val]=true;
                q.push(val);
            }
        }
    }

    cout<<"{";
    for(int i=0;i<path.size();i++)
    {
        if(i!=path.size()-1)
          cout<<path[i]<<" ";
        else
          cout<<path[i];
    }
    cout<<"}";
}

//DFS
void DFS(int i,vector<bool>&visited,vector<vector<int> >&paragraph)
{
    if(visited[i]) //DFS的出口，如果当前点已被访问则退出
      return;
    visited[i]=true;//将当前结点标记为已访问
    temppath.push_back(i);
    for(int val : paragraph[i])//访问邻居
    {
        if(!visited[val])//如果邻居未被访问则对邻居做DFS
        {
            DFS(val,visited,paragraph);
        }
    }
}
    
int main()
{
    int n,m;
    cin>>n>>m;
    vector<bool> visited(n,false);//辅助数组 用于记录该顶点是否被访问过
    vector<vector<int> > paragraph(n);//使用邻接表存储图结构
    for(int i=0;i<m;i++)
    {
        int a,b;
        cin>>a>>b;
        paragraph[a].push_back(b);
        paragraph[b].push_back(a);
    }
    for(int i=0;i<n;i++)
    {
        temppath.clear();
        DFS(i,visited,paragraph);
        if(temppath.size()>0)
        {
            cout<<"{";
        }
        for(int j=0;j<temppath.size();j++)
        {
            if(j!=temppath.size()-1)
              cout<<temppath[j]<<" ";
            else
              cout<<temppath[j];
        }
        if(temppath.size()>0)
        {
            cout<<"}";
        }
    }
    cout<<endl;
    fill(visited.begin(),visited.end(),false);
    for(int i=0;i<n;i++)
    {
        if(!visited[i])
        {
            BFS(i,visited,paragraph);
        }
    }
    cout<<endl;
}