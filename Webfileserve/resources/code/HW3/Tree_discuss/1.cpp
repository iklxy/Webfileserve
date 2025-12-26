#include<iostream>
using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    int height;
    TreeNode(int x) : val(x),left(nullptr),right(nullptr),height(1){};
};

const char* title="please choose the operation you want to do\n\
1.insert a node\n\
2.find the number of nodes in a range (please input low and high)\n\
3.inorder traversal\n\
4.reverse inorder traversal\n\
5.change the value of a node (please input old value and new value)\n";

class BTS
{
    public:
        TreeNode* root;
        BTS(TreeNode* r):root(r){};

        int getHeight(TreeNode* n) { return n ? n->height : 0; }

        int getBalance(TreeNode* n) 
        {
          return n ? getHeight(n->left) - getHeight(n->right) : 0;
        }

        TreeNode* leftRotate(TreeNode* x)
        {
            TreeNode* y=x->right;
            TreeNode* T2=y->left;
            y->left=x;
            x->right=T2;
            x->height=max(getHeight(x->left),getHeight(x->right))+1;
            y->height=max(getHeight(y->left),getHeight(y->right))+1;
            return y;
        }

        TreeNode* rightRotate(TreeNode* y)
        {
            TreeNode* x=y->left;
            TreeNode* T2=x->right;
            x->right=y;
            y->left=T2;
            y->height=max(getHeight(y->left),getHeight(y->right))+1;
            x->height=max(getHeight(x->left),getHeight(x->right))+1;
            return x;
        }

        TreeNode* insert(TreeNode* root,int val)
        {
            if(root==nullptr)
              return new TreeNode(val);
            if(val<root->val)
              root->left=insert(root->left,val);
            else if(val>root->val)
              root->right=insert(root->right,val);
            else
              return root;
              //更新高度
            root->height=1+max(getHeight(root->left),getHeight(root->right));
            //计算高度差
            int balance=getBalance(root);
            //不平衡的情况，执行旋转
            if(balance>1 && val<root->left->val)
              return rightRotate(root);
            if(balance<-1 && val>root->right->val)
              return leftRotate(root);
            if(balance>1 && val>root->left->val)
            {
                root->left=leftRotate(root->left);
                return rightRotate(root);
            }
            if(balance<-1 && val<root->right->val)
            {
                root->right=rightRotate(root->right);
                return leftRotate(root);
            }
            return root;
        }

        void inorder(TreeNode* root)
        {
            if(root==nullptr)
              return;
            inorder(root->left);
            cout<<root->val<<" ";
            inorder(root->right);
        }

        void reinorder(TreeNode* root)
        {
            if(root==nullptr)
              return;
            reinorder(root->right);
            cout<<root->val<<" ";
            reinorder(root->left);
        }

        void deleteNode(int val)
        {
            deleteHelper(this->root,val);
        }

        int countInRange(TreeNode* root, int low, int high)
        {
            if (root == nullptr)
                return 0;
        
            if (root->val < low)
                return countInRange(root->right, low, high);
        
            else if (root->val > high)
                return countInRange(root->left, low, high);
        
            else
                return 1 + countInRange(root->left, low, high)
                         + countInRange(root->right, low, high);
        }

    private:
        TreeNode* deleteHelper(TreeNode* root,int val)
        {
            if(root==nullptr)
              return nullptr;
            if(val<root->val)
              root->left=deleteHelper(root->left,val);
            else if(val>root->val)
              root->right=deleteHelper(root->right,val);
            else
            {
                if(root->left==nullptr && root->right==nullptr)
                {
                    delete root;
                    return nullptr;
                }
                else if(root->left==nullptr || root->right==nullptr)
                {
                    TreeNode* temp=root->left ? root->left : root->right;
                    delete root;
                    return temp;
                }
                else
                {
                    TreeNode* help=root->right;
                    while(help->left!=nullptr)
                      help=help->left;
                    root->val=help->val;
                    root->right=deleteHelper(root->right,help->val);
                }
            }
        }
};
int main()
{
    cout<<"Please input the value of the root node:(the number of BTS node is 10)"<<endl;
    int root_val;
    cin>>root_val;
    TreeNode* root=new TreeNode(root_val);
    BTS tree(root);
    for(int i=0;i<9;i++)
    {
        int val;
        cin>>val;
        tree.root=tree.insert(tree.root,val);
    }
    while(true)
    {
        cout<<title;
        int choice;
        cin>>choice;
        if(choice==1)
        {
            int val;
            cin>>val;
            tree.root=tree.insert(tree.root,val);
        }
        else if(choice==2)
        {
            int low,high;
            cin>>low>>high;
            int count=tree.countInRange(tree.root,low,high);
            cout<<"Number of nodes in range ["<<low<<","<<high<<"]: "<<count<<endl;
        }
        else if(choice==3)
        {
            tree.inorder(tree.root);
            cout<<endl;
        }
        else if(choice==4)
        {
            tree.reinorder(tree.root);
            cout<<endl;
        }
        else if(choice==5)
        {
            int old_val,new_val;
            cin>>old_val>>new_val;
            tree.deleteNode(old_val);
            tree.root=tree.insert(tree.root,new_val);
        }
    }
    return 0;
}