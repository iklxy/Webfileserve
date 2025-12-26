/*感染二叉树所需要的时间*/
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

struct TreeNode
{
    int left;
    int right;
    TreeNode():left(-1),right(-1){};
};

int BFS(int n,vector< vector<int> >&G,int start)
{
    //Translate的辅助函数，使用BFS遍历无向图，计算感染所需时间
    int count=0;
    //visited记录该结点是否被感染过
    vector<bool>visited(n,false);
    //队列用于BFS遍历
    queue<int>q;
    //起始感染结点入队
    q.push(start);
    visited[start]=true;

    while(!q.empty())
    {
        //Jud用于判断本层是否有新节点被感染，有的情况下才增加时间
        bool Jud=false;
        int level=q.size();
        for(int i=0;i<level;i++)
        {
            int Tree=q.front();
            q.pop();
            for(int val : G[Tree])
            {
                if(!visited[val])
                {
                    //该结点未被感染过，则感染该结点，且入队
                    visited[val]=true;
                    q.push(val);
                    Jud=true;
                }
            }
        }
        //本层有新节点被感染，时间加1
        if(Jud) count++;
    }
    return count;
}
int Translate(vector<TreeNode>&T,int start)
{
    //将此二叉树转换为无向临接表
    vector<vector<int>>graph(T.size());
    for(int i=0;i<T.size();i++)
    {
        if(T[i].left!=-1)
        {
            //当前结点的邻居加入左孩子，左孩子的邻居加入当前结点
            graph[i].push_back(T[i].left);
            graph[T[i].left].push_back(i);
        }
        if(T[i].right!=-1)
        {
            //当前结点的邻居加入右孩子，右孩子的邻居加入当前结点
            graph[i].push_back(T[i].right);
            graph[T[i].right].push_back(i);
        }
    }
    //使用BFS遍历无向图，计算感染所需时间
    //为何使用BFS？因为感染时间一定是最短的，而BFS恰好能找到最短路径
    return BFS(T.size(),graph,start);
}

int main()
{
    int n,start;
    cin>>n>>start;
    
    vector<TreeNode>Tree(n);

    for(int i=0;i<n;i++)
    {
        int ChildLeft=0;
        int ChildRight=0;
        cin>>ChildLeft>>ChildRight;
        Tree[i].left=ChildLeft;
        Tree[i].right=ChildRight;
    }

    cout<<Translate(Tree,start);
}