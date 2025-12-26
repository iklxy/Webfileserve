/*给出二叉树的前序中序遍历，返回后序遍历*/
#include<iostream>
#include<string>
using namespace std;

string GetPostorder(const string&Preorder,const string&Inorder)
{
    if (Preorder.empty() || Inorder.empty())
      return "";

    char root = Preorder[0];
    int index = Inorder.find(root);
    if (index == string::npos)
      return "#";
  
    string leftPre  = Preorder.substr(1, index);
    string leftIn   = Inorder.substr(0, index);
    string rightPre = Preorder.substr(1 + index);
    string rightIn  = Inorder.substr(index + 1);
  
    string leftPost  = GetPostorder(leftPre, leftIn);
    string rightPost = GetPostorder(rightPre, rightIn);
  
    if (leftPost == "#" || rightPost == "#")
      return "#"; 
  
    return leftPost + rightPost + root;
}

int main()
{
  //两个string变量分别存储前序和中序遍历结果
    string Preorder,Inorder;
    while(cin>>Preorder>>Inorder)
    {
        string Postorder=GetPostorder(Preorder,Inorder);
        if(Postorder=="#")
          cout<<"Error"<<endl;
        else
          cout<<Postorder<<endl;
    }
}
