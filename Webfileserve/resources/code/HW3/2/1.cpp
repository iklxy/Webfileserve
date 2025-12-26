/*判断两棵树是否同构*/
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<stack>
using namespace std;

struct TreeNode
{
    int left;//左孩子
    int right;//右孩子
    char data;//存储的值
    TreeNode():right(-1),left(-1){};
};

int FindRoot(vector<TreeNode> &T)
{
  //这是JudIsomorphism的辅助函数，用于寻找并返回一棵树的根节点
    int root=0;
    vector<bool> Jud(T.size(),true);
    for(int i=0;i<T.size();i++)
    {
        if(T[i].left!=-1)
          Jud[T[i].left]=false;
        if(T[i].right!=-1)
          Jud[T[i].right]=false;
    }
    for(int i=0;i<T.size();i++)
    {
        if(Jud[i]==true)
          root=i;
    }
    return root;
}

bool JudIsomorphism(int root_1,int root_2,vector<TreeNode>&T1,vector<TreeNode>&T2)
{
  //考虑使用递归函数来解决问题
  //root_1和root_2分别是两棵树的根节点，T1和T2分别是两棵树的存储结构
  //当左右对应的结点均空，则同构
    if(root_1==-1 && root_2==-1)
      return true;
  //一个为空一个非空，显然不同构
    if(root_1==-1 || root_2==-1)
      return false;
  //对应结点的值不同，显然不同构
    if(T1[root_1].data!=T2[root_2].data)
      return false;
  //分别获取左右孩子的下标
    int L1=T1[root_1].left;
    int R1=T1[root_1].right;
    int L2=T2[root_2].left;
    int R2=T2[root_2].right;
  //分别判断左右孩子交换的情况和不交换的情况
    bool swap=JudIsomorphism(L1,R2,T1,T2) && JudIsomorphism(R1,L2,T1,T2);
    bool noswap=JudIsomorphism(L1,L2,T1,T2) && JudIsomorphism(R1,R2,T1,T2);
  //只要交换或不交换中的一种情况成立就说明两棵树是同构的
    return swap || noswap;
}

int getDepth1(int root,vector<TreeNode>&tree1)
 {
    if (root == -1) return 0;
    
    int leftDepth = getDepth1(tree1[root].left,tree1);
    int rightDepth = getDepth1(tree1[root].right,tree1);
    
    return max(leftDepth, rightDepth) + 1;
}

int getDepth2(int root,vector<TreeNode>&tree2)
 {
    if (root == -1) return 0;
    
    int leftDepth = getDepth2(tree2[root].left,tree2);
    int rightDepth = getDepth2(tree2[root].right,tree2);
    
    return max(leftDepth, rightDepth) + 1;
}

int main()
{
    int n;//树的节点个数
    cin>>n;
    cin.ignore(); 
    vector<TreeNode> Tree_1(n);
    string input;
    for(int i=0;i<n;i++)
    {
        getline(cin,input);//读取每一行的命令
        istringstream os(input);
        string s1,s2;
        os>>Tree_1[i].data>>s1>>s2;

        if(s1=="-")
          Tree_1[i].left=-1;
        else
          Tree_1[i].left=stoi(s1);
        
        if(s2=="-")
          Tree_1[i].right=-1;
        else
          Tree_1[i].right=stoi(s2);
    }
    cin>>n;
    cin.ignore(); 
    vector<TreeNode> Tree_2(n);
    for(int i=0;i<n;i++)
    {
        getline(cin,input);
        istringstream os(input);
        string s1,s2;
        os>>Tree_2[i].data>>s1>>s2;

        if(s1=="-")
          Tree_2[i].left=-1;
        else
          Tree_2[i].left=stoi(s1);
        
        if(s2=="-")
          Tree_2[i].right=-1;
        else
          Tree_2[i].right=stoi(s2);
    }

    int root1=FindRoot(Tree_1);
    int root2=FindRoot(Tree_2);

    if(Tree_1.size()!=Tree_2.size())
    {
        cout<<"No"<<endl;
        cout<<getDepth1(root1,Tree_1)<<endl;
        cout<<getDepth2(root2,Tree_2)<<endl;
        return 0;
    }
    
    if(JudIsomorphism(root1,root2,Tree_1,Tree_2))
      cout<<"Yes"<<endl;
    else
      cout<<"No"<<endl;

    cout<<getDepth1(root1,Tree_1)<<endl;
    cout<<getDepth2(root2,Tree_2)<<endl;
    return 0;
}