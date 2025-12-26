/*某校的计算机系有 n 门必修课程。学生需要修完所有必修课程才能毕业。
每门课程都需要一定的学时去完成。有些课程有前置课程，需要先修完它们才能修这些课程；而其他课程没有。 不同于大多数学校，学生可以在任何时候进行选课，且同时选课的数量没有限制。
现在校方想要知道：
从入学开始，每门课程最早可能完成的时间（单位：学时）；
对每一门课程，若将该课程的学时增加1，是否会延长入学到毕业的最短时间。*/

//题目已经人为保证了没有cycle，否则永远不可能完成课程
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;

struct Edge
{
    int u;//前置课程
    int v;//后置课程
    int wight;//学时
};


//正向拓扑排序，以入度来进行遍历
vector<int> topo(vector<int>&indegree,vector<Edge>&graph)
{
    int n=indegree.size()-1;
    vector<int>topo_order(n+1,0);
    queue<int>q;
    for(int i=0;i<=n;i++)
    {
        if(indegree[i]==0)
          q.push(i);
    }
    while(!q.empty())
    {
        int current=q.front();
        q.pop();
        topo_order.push_back(current);
        for(auto&edge : graph)
        {
            if(edge.u==current)
            {
                indegree[edge.v]--;
                if(indegree[edge.v]==0)
                  q.push(edge.v);
            }
        }
    }
    return topo_order;
}

//逆拓扑排序，以出度来进行遍历
vector<int> retopo(vector<int>&outdegree,vector<Edge>&graph)
{
    int n=outdegree.size()-1;
    vector<int>retopo_order(n+1,0);
    queue<int>q;
    for(int i=0;i<=n;i++)
    {
        if(outdegree[i]==0)
          q.push(i);
    }
    while(!q.empty())
    {
        int current=q.front();
        q.pop();
        retopo_order.push_back(current);
        for(auto&edge : graph)
        {
            if(edge.v==current)
            {
                outdegree[edge.u]--;
                if(outdegree[edge.u]==0)
                  q.push(edge.u);
            }
        }
    }
    return retopo_order;
}

//通过dp计算活动的最早发生时间
void dp_ve(vector<int>&ve,vector<int>&topo_order,vector<Edge>&graph)
{
    for(int u : topo_order)
    {
        for(auto&edge : graph)
        {
            if(edge.u==u)
            {
                //由u指向v v的最早发生时间是 当前已有的v的最早发生时间 和 u的最早发生时间 + 边的权重 的最大值
                ve[edge.v]=max(ve[edge.v],ve[u]+edge.wight);
            }
        }
    }
}

//dp计算活动的最迟发生时间
void dp_vl(vector<int>&vl,vector<int>&retopo_order,vector<Edge>&graph,int max_time)
{
    //先做一步初始化，这里的max_time是ve中的最大值，因为汇点的最早发生时间和最迟发生时间是一样的
    for(int i=0;i<vl.size();i++)
    {
        vl[i]=max_time;
    }
    for(int v : retopo_order)
    {
        for (auto& edge : graph)
        {
            if (edge.v == v)
            {
                //由u指向v的边 u的最迟发生时间等于 当前已有的u的最迟发生时间 和 v的最迟发生时间减去边的权重 的最小值
                vl[edge.u] = min(vl[edge.u], vl[edge.v] - edge.wight);
            }
        }
    }
}

int main()
{
    int n;
    cin>>n;
    vector<Edge>graph(n*(n-1)/2);//存储图结构
    vector<int>indegree(n+1,0);//存储入度
    vector<int>outdegree(n+1,0);//存储出度

    for(int i=1;i<=n;i++)
    {
        int time;
        cin>>time;
        int course_num;
        cin>>course_num;
        if(course_num==0)
        {
            Edge e;
            e.u=0;
            e.v=i;
            e.wight=time;
            graph.push_back(e);
            indegree[i]++;
            outdegree[0]++;
            continue;
        }
        for(int j=1;j<=course_num;j++)
        {
            int pre_course;
            cin>>pre_course;
            Edge e;
            e.u=pre_course;
            e.v=i;
            e.wight=time;
            graph.push_back(e);
            indegree[i]++;
            outdegree[pre_course]++;
        }
    }

    vector<int>topo_order(n+1,0);
    topo_order = topo(indegree,graph);
    vector<int>retopo_order(n+1,0);
    retopo_order = retopo(outdegree,graph);

    vector<int>ve(n+1,0);
    vector<int>vl(n+1,0);

    dp_ve(ve,topo_order,graph);//earlies time
    int max_time = 0;
    for(int i = 1; i <= n; i++) max_time = max(max_time, ve[i]);
    dp_vl(vl,retopo_order,graph,max_time);//late time

    vector<int>e(n+1,0);
    vector<int>l(n+1,0);

    //计算事件的最早发生时间和最迟发生时间 活动盈余等于 l-e l-e等于0的点在关键路径上
    for(int i=0;i<n+1;i++)
    {
        e[graph[i].v]=ve[graph[i].u];
        l[graph[i].v]=vl[graph[i].v]-graph[i].wight;
    }

    for(int i=1;i<=n;i++)
    {
        cout<<ve[i]<<" ";
        if(ve[i]==vl[i])
          cout<<1<<endl;
        else
          cout<<0<<endl;
    }
    return 0;
}