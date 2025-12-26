//BST
#include <iostream>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode* left;//左孩子
    TreeNode* right;//右孩子
    TreeNode(int x):val(x),left(nullptr),right(nullptr){};
};

void insert(TreeNode* &root,int val)
{
    if(val<root->val)
    {
        if(root->left==nullptr)
        {
            root->left=new TreeNode(val);
        }
        else
        {
            insert(root->left,val);
        }
    }
    else
    {
        if(root->right==nullptr)
        {
            root->right=new TreeNode(val);
        }
        else
        {
            insert(root->right,val);
        }
    }
}

//以删除为例
//返回值表示是否删除成功
bool delete_tree(TreeNode* &root,int val)
{
    if(root==nullptr)
    {
        return false;
    }
    if(root->val==val)
    {
        //是叶子节点，直接删除就好
        if(root->left==nullptr && root->right==nullptr)
        {
            delete root;
            root=nullptr;
        }
        //有左孩子或者右孩子 选取孩子节点来替代即可
        else if(root->left == nullptr || root->right == nullptr)
        {
            TreeNode* temp=root;
            root=(root->left==nullptr)?root->right:root->left;
            delete temp;
            temp=nullptr;
        }
        else//有左右孩子，选择右子树的最小值来替代
        {
            TreeNode* temp=root->right;
            while(temp->left!=nullptr)
            {
                temp=temp->left;
            }
            root->val=temp->val;
            delete_tree(root->right,temp->val);
        }
        return true;
    }
    else if(val<root->val)//去左子树找
    {
        return delete_tree(root->left,val);
    }
    else//去右子树找
    {
        return delete_tree(root->right,val);
    }
}

int find_tree_sum(TreeNode* root,int val)
{
    if(root==nullptr)
      return 0;

    if(root->val<val)
    {
        return find_tree_sum(root->right,val);
    }
    else if(root->val>val)
    {
        return find_tree_sum(root->left,val);
    }
    else
    {
        return 1+find_tree_sum(root->right,val);
    }
}

int find_min(TreeNode* root)
{
    if(root->left==nullptr)
      return root->val;
    else 
      return find_min(root->left);
}

int find_priority(TreeNode* root,int val,int &res)
{
    bool found = false;
    TreeNode* curr = root;
    while (curr != nullptr)
    {
        if (curr->val < val)
        {
            // 当前值比目标小，是一个候选答案
            res = curr->val;
            found = true;
            curr = curr->right;
        }
        else
        {
            curr = curr->left;
        }
    }
    return found;
}

int main()
{
    int operation_num;
    cin>>operation_num;
    TreeNode* root = new TreeNode(0);

    for(int i=0;i<operation_num;i++)
    {
        int op;
        int num=0;
        cin>>op;
        if(op!=4)
          cin>>num;
        if(op==1)
        {
            if(root->val==0)
            {
                root->val=num;
            }
            else
            {
                insert(root,num);
            }
        }
        else if(op==2)
        {
            int res=delete_tree(root,num);
            if(res==false)
              cout<<"None"<<endl;
        }
        else if(op==3)
        {
            int res=find_tree_sum(root,num);
            cout<<res<<endl;
        }
        else if(op==4)
        {
            int min=find_min(root);
            cout<<min<<endl;
        }
        else if(op==5)
        {
            int res=0;
            if(find_priority(root,num,res)==true)
            {
                cout<<res<<endl;
            }
            else
            {
                cout<<"None"<<endl;
            }
        }
    }
}