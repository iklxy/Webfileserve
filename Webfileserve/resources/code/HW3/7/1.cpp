/*构建表达式树*/
#include<iostream>
#include<stack>
#include<string>
#include<map>
#include<cctype>
#include<cmath>
#include<vector>
using namespace std;

struct TreeNode
{
    char val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char v):val(v),left(nullptr),right(nullptr){};
};

void makeSubTree(stack<TreeNode*>&Tree, stack<char>&ops);
int precedence(char op);
TreeNode* BuildExpressionTree(const string& expression);
void Postorder(TreeNode* root);
int getHeight(TreeNode* root) ;
void fillMatrix(vector<string>& canvas, TreeNode* root, int row, int col, int gap) ;
void printTree(TreeNode* root);
int evaluate(TreeNode*root,map<char,int>&valMap);

void makeSubTree(stack<TreeNode*>&Tree, stack<char>&ops)
{
    //拿到栈顶操作符，弹出栈顶操作符
    char op=ops.top();
    ops.pop();
    //弹出栈顶的两个树结点，建立以该操作符为根节点的子树，并将子树根节点入栈
    TreeNode* right=Tree.top();
    Tree.pop();
    TreeNode* left=Tree.top();
    Tree.pop();
    TreeNode* newNode=new TreeNode(op);
    newNode->right=right;
    newNode->left=left;
    Tree.push(newNode);
}

int precedence(char op)
{
    if(op=='+' || op=='-')
      return 1;
    if(op=='*' || op=='/')
      return 2;
    return 0;
}

TreeNode* BuildExpressionTree(const string& expression)
{
    //用栈存储树的结点和操作符
    stack<TreeNode*>Tree;
    stack<char>ops;
    //编译表达式
    for(char c : expression)
    {
        if(isspace(c))
          continue;
        if(isalpha(c))
        {
            //当字符是操作数时，建立新结点并且入栈
            TreeNode* newNode=new TreeNode(c);
            Tree.push(newNode);
        }
        else if(c=='(')//左括号直接入栈
          ops.push(c);
        else if(c==')')
        {
            //和之前布尔表达式类似，遇到右括号则不断弹出操作符并建立子树，直到遇到左括号为止
            while(!ops.empty() && ops.top()!='(')
              makeSubTree(Tree,ops);
            ops.pop();
            //此处弹出左括号
        }
        else
        {
            //当入栈操作符优先级小于等于栈顶操作符时，不断弹出操作符并建立子树
            while(!ops.empty() && precedence(ops.top())>=precedence(c))
              makeSubTree(Tree,ops);
            ops.push(c);
        }
    }
    //将剩余操作符依次弹出并建立子树
    while(!ops.empty())
      makeSubTree(Tree,ops);
    //最终栈顶元素即为表达式树的根节点，返回根节点
    return Tree.top();
}

void Postorder(TreeNode* root)
{
    if(root==nullptr)
      return;
    Postorder(root->left);
    Postorder(root->right);
    cout<<root->val;
}

int getHeight(TreeNode* root) 
{
    if (!root) return 0;
      return 1 + max(getHeight(root->left), getHeight(root->right));
}

void fillMatrix(vector<string>& canvas, TreeNode* root, int row, int col, int gap) 
{
    if (!root) return;

    canvas[row][col] = root->val;

    if (root->left) 
    {
        canvas[row + 1][col - 1] = '/';
        fillMatrix(canvas, root->left, row + 2, col - gap, gap / 2);
    }
    if (root->right) 
    {
        canvas[row + 1][col + 1] = '\\';
        fillMatrix(canvas, root->right, row + 2, col + gap, gap / 2);
    }
}

void printTree(TreeNode* root) 
{
    if (!root) return;

    int h = getHeight(root);
    int maxWidth = (int)pow(2, h); 
    //将每一行的内容视为一个字符串
    vector<string> canvas(2 * h - 1, string(maxWidth, ' '));

    int rootCol = maxWidth / 2-1;  
    int gap = (int)pow(2, h - 2); 
    //递归填充canvas矩阵，即每一行的内容
    fillMatrix(canvas, root, 0, rootCol, gap);

    for (string& line : canvas) 
    {
        //去掉行末多余的空格，使用string类的find_last_not_of函数
        //此时从行末开始查找，查找第一个不是空格的位置
        int end = line.find_last_not_of(' ');
        if (end != string::npos)
            line = line.substr(0, end + 1);
        else
            line.clear();
        cout << line << endl;
    }
}

int evaluate(TreeNode*root,map<char,int>&valMap)
{
    //使用了map存储变量对应的值
    //操作符为key值，其真值为value值
    if(root==nullptr)
      return 0;
    if(isalpha(root->val))
      return valMap[root->val];
    int leftvalue=evaluate(root->left,valMap);
    int rightvalue=evaluate(root->right,valMap);
    switch(root->val)
    {
        case '+':
          return leftvalue+rightvalue;
        case '-':
          return leftvalue-rightvalue;
        case '*':
          return leftvalue*rightvalue;
        case '/':
          return leftvalue/rightvalue;
    }
}

int main()
{
    string expression;
    cin>>expression;
    TreeNode* root=BuildExpressionTree(expression);
    int n;
    cin>>n;

    map<char,int>valMap;
    for(int i=0;i<n;i++)
    {
        char val;
        cin>>val;
        int num;
        cin>>num;
        valMap[val]=num;
    }
    Postorder(root);
    cout<<endl;
    printTree(root);
    cout<<evaluate(root,valMap)<<endl;
}