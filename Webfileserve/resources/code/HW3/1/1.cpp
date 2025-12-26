/*二叉树的非递归遍历 已知中序非递归遍历，输出其后序遍历的答案*/
#include<iostream>
#include<vector>
#include<string>
#include<stack>
using namespace std;

struct TreeNode
{
    char value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char val):value(val),left(nullptr),right(nullptr){};//构造函数
};

void postorder(TreeNode* root)
{
    if(root!=nullptr)
    {
        postorder(root->left);
        postorder(root->right);
        cout<<root->value;
    }
    else
      return;
}

int main()
{
    int n;//二叉树节点的个数 对于每个节点 可执行的操作有pop和push两种
    //因此负责操作的vector数组大小均为2*n 带值的vector数组大小为n
    vector<string> operation(2 * n);
    vector<char> value(n);

    cin>>n;
    for(int i=0;i<2*n;i++)
    {
        cin>>operation[i];
        if(operation[i]=="push")
          cin>>value[i];
    }

    //初始化一个栈用来还原二叉树
    stack<TreeNode*>Tree;
    //根节点
    TreeNode* root=nullptr;
    //遍历中临时存储被pop的节点
    TreeNode* last=nullptr;
    for(int i=0;i<2*n;i++)
    {
        //当遇到operation为push时，说明该结点是当前栈顶结点的左孩子或者右孩子
        if(operation[i]=="push")
        {
            //建立一个新结点
            TreeNode* temp=new TreeNode(value[i]);
            //如果根节点为空，则将该结点设为根节点
            if(!root)root=temp;
            //在栈非空的情况下
            if(!Tree.empty())
            {
                //如果栈顶结点无左孩子，说明该结点是其左孩子
                if(!Tree.top()->left)
                  Tree.top()->left=temp;
                //否则该结点是其右孩子
                else
                  last->right=temp;
            }
            //特判，当栈为空时，说明pop了根节点，此时last指向的结点是根节点
            if(last!=nullptr && Tree.empty())
              last->right=temp;
            Tree.push(temp);//入栈
        }
        else
        {
            //遇到pop操作时，让栈顶元素出栈，但此时需要last记录下被pop的节点，以便下一次push操作时能正确连接右孩子
            last=Tree.top();
            Tree.pop();
        }
    }
    postorder(root);
}
