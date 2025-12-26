/*假设无向图G上有N个点和M条边，点编号为1到N，第i条边长度为w_i，其中H个点上有可以食用的牧草。
另外有R匹小马，第j匹小马位于点start_j，需要先前往任意一个有牧草的点进食牧草，然后前往点end_j，
请你计算每一匹小马需要走过的最短距离。*/
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int I_MAX = 21474836;

// 邻接表结构
struct Edge 
{
    int v;      // 终点
    int weight; // 边权
};

//Dijkstra算法
void Dijkstra(int start_node, int N, const vector<vector<Edge> >& adj, vector<int>& path_length) 
{
    // 初始化距离数组
    fill(path_length.begin(), path_length.end(), I_MAX);

    // pair<distance, node>
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    
    //初始点到自己的距离是0
    path_length[start_node] = 0;
    
    pq.push(std::make_pair(0, start_node));

    while (!pq.empty())
     {
        auto temp = pq.top();
        pq.pop();
        
        int d = temp.first; // 距离
        int u = temp.second; // 节点
        
        //如果当前取出的距离 d 比已知最短距离大，则忽略
        if (d > path_length[u]) 
        {
            continue;
        }

        for (const auto& edge : adj[u]) 
        {
            int v = edge.v;
            int w = edge.weight;

            //如果起始点到当前结点u的距离加上权重 小于 v点的路径长度 则更新v点的路径长度 然后入队
            if ((long long)path_length[u] + w < path_length[v]) //用long long 是防止溢出
            {
                path_length[v] = path_length[u] + w;
                pq.push(std::make_pair(path_length[v], v));
            }
        }
    }
}

int main() 
{
    int N, M; // N点，M边
    if (!(cin >> N >> M)) return 0;

    vector<vector<Edge> > adj(N + 1); 

    for (int i = 0; i < M; i++) {
        int u, v, val;
        cin >> u >> v >> val;

        adj[u].push_back({v, val});
        adj[v].push_back({u, val});
    }

    int H, R; // 牧草点数量 H，小马数量 R
    if (!(cin >> H >> R)) return 0;

    vector<int> grass_nodes(H);
    for (int i = 0; i < H; i++) {
        cin >> grass_nodes[i];
    }
    
    // 只对 H 个牧草点运行 Dijkstra
    vector<vector<int> > dist_from_grass(H, vector<int>(N + 1)); 

    for (int i = 0; i < H; i++) 
    {
        int start_node = grass_nodes[i];
        Dijkstra(start_node, N, adj, dist_from_grass[i]);
    }

    // 查询 R 匹小马
    for (int i = 0; i < R; i++) 
    {
        int start, end;
        cin >> start >> end;
        
        int min_length = I_MAX;

        // 遍历所有 H 个牧草点 G_j
        for (int j = 0; j < H; j++) 
        {
            
            int d_start_to_grass = dist_from_grass[j][start]; // dist(G_j, S)
            int d_grass_to_end = dist_from_grass[j][end];     // dist(G_j, E)

            if (d_start_to_grass != I_MAX && d_grass_to_end != I_MAX) 
            {
                 if ((long long)d_start_to_grass + d_grass_to_end < min_length) 
                 {
                    min_length = d_start_to_grass + d_grass_to_end;
                 }
            }
        }
        cout << min_length <<endl;
    }
    return 0;
}