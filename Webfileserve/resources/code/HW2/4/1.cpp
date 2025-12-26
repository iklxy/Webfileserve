/*BFS*/
//this problem request that the island is surrounded by water
#include<iostream>
#include<vector>
#include<queue>
using namespace std;
//Directions
int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};

//BFS
void BFS(int x,int y,vector<vector<bool>>&grid)
{
    int Row=grid.size();
    int Col=grid[0].size();
    queue<pair<int,int>>q;
    q.push({x,y});
    grid[x][y]=0;
    while(!q.empty())
    {
        auto cur=q.front();
        q.pop();
        for(int i=0;i<4;i++)
        {
            int tx;
            int ty;
            tx=cur.first+dx[i];
            ty=cur.second+dy[i];
            if(tx>=0 && tx<=Row-1 && ty>=0 && ty<=Col-1 && grid[tx][ty])
            {
                q.push({tx,ty});
                grid[tx][ty]=0;
            }
        }
    }
}

//get island number function
int getIslandNum(vector<vector<bool>>&grid)
{
    //Row and Col
    int Row=grid.size();
    int Col=grid[0].size();
    //Num
    int num=0;
    //BFS
    for(int i=1;i<Row-1;i++)
    {
        for(int j=1;j<Col-1;j++)
        {
            if(grid[i][j])
            {
                num++;
                BFS(i,j,grid);
            }
        }
    }
    return num;
}

int main()
{
    //Input
    int m,n;
    cin>>m>>n;
    vector<vector<bool>>grid(m,vector<bool>(n,0));
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            bool val;
            cin>>val;
            if(val)grid[i][j]=1;
        }
    }
    //get island number
    int islandNum=getIslandNum(grid);
    cout<<islandNum<<endl;
}