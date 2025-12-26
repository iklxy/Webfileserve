/*树的重构*/
#include<iostream>
#include<vector>
#include<stack>
#include<string>
#include<algorithm>
using namespace std;

struct TreeNode
{
    int left_child;
    int right_brother;
    int last_child;//记录当前节点的最后一个孩子节点，方便插入新的兄弟节点
    TreeNode():left_child(-1),right_brother(-1),last_child(-1){};
};

int main()
{
    string line;
    int num=1;
    while(true)
    {
        vector<TreeNode> Tree(10005);
        //注意这里的栈s的初始值为1，所以根节点的编号为1开始
        stack<int>s;
        s.push(1);
        int count=1;
        int h1=0;
        getline(cin,line);
        if(line.empty())
          continue;
        if(line[0]=='#')
          break;
        //循环读取每一行DFS遍历中的'u'和'd'
        for(char c : line)
        {
            //父结点是栈的顶元素
            int parent=s.top();
            if(c=='d')
            {
                //如果操作是下行，则先将count++，代表新结点的编号
                count++;
                //如果父结点的左孩子为空，则将新结点设为左孩子
                //否则将新结点设为当前父结点最后一个孩子的右兄弟
                if(Tree[parent].left_child==-1)
                {
                    Tree[parent].left_child=count;
                }
                else
                {
                    Tree[Tree[parent].last_child].right_brother=count;
                }
                //更新父结点的最后一个孩子为当前新结点
                Tree[parent].last_child=count;
                s.push(count);
                //更新树的高度
                h1=max(h1,(int)s.size()-1);
            }
            else if(c=='u')
            {
                //如果操作是上行，则将栈顶元素弹出
                if(s.size() > 1)       
                    s.pop();
            }
        }

        //求转换后二叉树的高度
        int h2 = 0;
        stack <pair <int,int> > s1;
        s1.push(make_pair(1, 0));  // (节点索引, 深度)
        while (!s1.empty()) 
        {
            pair<int, int> topPair = s1.top();
            s1.pop();
            int u = topPair.first;
            int d = topPair.second;
        z
            if (d > h2)
                h2 = d;

            if (Tree[u].left_child != -1)
                s1.push(make_pair(Tree[u].left_child, d + 1));
            
            if (Tree[u].right_brother != -1)
                s1.push(make_pair(Tree[u].right_brother, d + 1));
        }
        cout<<"Tree "<<num++<<": "<<h1<<" => "<<h2<<endl;
    }
}