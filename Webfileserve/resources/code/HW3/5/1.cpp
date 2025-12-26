/*求公共祖先*/
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<sstream>
using namespace std;

int FindAncestor(const vector<int>& parent, int x, int y)
{
    //x和y分别为需要查询的两个结点
    //使用visited数组记录从x到根节点的路径上的所有父结点
    vector<bool> visited(parent.size(), false);
    int cur = x;
    while (cur != 0) 
    {
        //将此时的结点标记为已访问
        visited[cur] = true;
        //更新当前结点为其父节点
        cur = parent[cur];
    }

    cur = y;
    while (cur != 0) 
    {
        //当y的某个父节点在x的路径上出现时，说明该节点为公共祖先，返回其值
        if (visited[cur]) return cur;
        //更新当前结点为其父节点
        cur = parent[cur];
    }
    return -1;
}

int main()
{
    int T=0;
    cin>>T;
    for(int i=0;i<T;i++)
    {
        int root=0;
        int N,M;
        cin>>N>>M;

        //用临接矩阵存储结点的孩子
        vector<vector<int>>children(N+1);
        //用数组存储结点的父节点
        vector<int>parent(N+1);
        //辅助数组，判断某节点是否有父节点，便于后续寻找根节点
        vector<bool>HasParent(N+1,false);

        for(int i=0;i<N-1;i++)
        {
            int p,c;
            cin>>p>>c;
            children[p].push_back(c);
            parent[c]=p;
            HasParent[c]=true;
        }

        for (int i = 1; i <= N; i++)
         {
            if (!HasParent[i]) 
            {
                root = i;
                break;
            }
        }

        for(int i=0;i<M;i++)
        {
            int x,y;
            cin>>x>>y;
            if(x==root || y==root)
            {
                cout<<root<<endl;
                continue;
            }
            cout<<FindAncestor(parent,x,y)<<endl;
        }
    }
}