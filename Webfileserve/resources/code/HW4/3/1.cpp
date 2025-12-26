/*N个村庄，从1到N编号，现在请你修建一些路使得任何两个村庄都彼此连通。我们称两个村庄A和B是连通的，当且仅当在A和B之间存在一条路，或者存在一个村庄C，使得A和C之间有一条路，并且C和B是连通的。

已知在一些村庄之间已经有了一些路，您的工作是再兴建一些路，使得所有的村庄都是连通的，并且新建的路的长度是最小的。*/
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge//边结构体
{
    int u;//边的头
    int v;//边的尾
    int wight;//边的权重
    //对Edge类型重载了 < 运算符,便于在后续构建最小生成树的时候直接通过边的权值对边进行排序
    bool operator<(const Edge& other) const;  
};

//kruskal算法类
class kruskal
{
public:
    vector<int> parent;//parent数组，用于记录各结点的祖先结点
    vector<int> rank;//结点的秩数组，用于记录各结点的秩，用于在union联合时判断边的方向

    kruskal(int n);             
    int find(int x);//查找某一节点的祖先结点       
    void union_(int x,int y);//将x 和 y联合为一条边加入到最小生成树中
};

bool Edge::operator <(const Edge& other) const
{
    return wight < other.wight;
}

kruskal::kruskal(int n)
{
    parent.resize(n + 1);
    rank.resize(n + 1, 0);
    for (int i = 1; i <= n; i++)
        parent[i] = i;//每一个结点的初始祖先都是自己
}

int kruskal::find(int x)
{
    return parent[x] == x ? x : parent[x] = find(parent[x]);//递归查找祖先
}


void kruskal::union_(int x, int y)
{
    int rootX = find(x);//找到x的祖先
    int rootY = find(y);//找到y的祖先

    if (rootX != rootY)//如果x的祖先和y的祖先不一致，说明x和y连接成为边此时并不在当前的树集合中
    {
        if (rank[rootX] < rank[rootY])//如果x的秩小于y的秩，则x的祖先是y
        {
            parent[rootX] = rootY;
        }
        else
        {
            parent[rootY] = rootX;//反之y的祖先是x
            if (rank[rootX] == rank[rootY])//在两个点的秩相同时，因为在上一行代码选择了x为祖先
            //所以此时祖先结点秩+1用于区分
                rank[rootX]++;
        }
    }
}

int main()
{
    int n;
    cin>>n;
    vector<Edge> edges(n*(n-1)/2);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            int w;
            cin>>w;
            if(j<=i) continue;
            if(w!=0)
            {
                Edge e;
                e.u=i+1;
                e.v=j+1;
                e.wight=w;
                edges.push_back(e);
            }
        }
    }
    kruskal ks(n);
    int p;
    cin>>p;
    for(int i=0;i<p;i++)
    {
        int a,b;
        cin>>a>>b;
        for(auto &edge : edges)
        {
            if((edge.u==a && edge.v==b) || (edge.u==b && edge.v==a))
            {
                edge.wight=0;
                break;
            }
        }
    }
    sort(edges.begin(),edges.end());
    int totalwight=0;
    //从权重最小的边开始构造最小生成树
    for(auto &edge : edges)
    {
        int u=edge.u;
        int v=edge.v;
        int w=edge.wight;
        //当两个结点的祖先不一致时，加入树集合
        if(ks.find(u)!=ks.find(v))
        {
            ks.union_(u,v);
            totalwight+=w;
        }
    }
    cout<<totalwight<<endl;
    return 0;
}