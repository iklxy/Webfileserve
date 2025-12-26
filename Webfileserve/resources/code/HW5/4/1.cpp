/*家族树*/
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

struct TreeNode
{
    string name;//节点名字
    TreeNode *parent;//父节点
    int depth;//节点深度
    TreeNode(string name, TreeNode *parent, int depth) : name(name), parent(parent), depth(depth) {};
};

int main()
{
    int N, M;

    while (cin >> N >> M && (N != 0 || M != 0))
    {
        //构建哈希表
        map<string, TreeNode *> family_tree;
        vector<TreeNode *> activate_tree(5050, nullptr);

        string line;
        getline(cin, line);

        for (int i = 0; i < N; i++)
        {
            getline(cin, line);
            int space = 0;

            while (space < line.length() && line[space] == ' ')
            {
                space++;
            }

            string name = line.substr(space);

            if (!name.empty() && name.back() == '\r')
            {
                name.pop_back();
            }

            TreeNode *parent = nullptr;
            if (space > 0)
            {
                parent = activate_tree[space - 1];
            }
            TreeNode *current = new TreeNode(name, parent, space);
            family_tree[name] = current;
            activate_tree[space] = current;
        }

        for (int i = 0; i < M; i++)
        {
            string question;
            getline(cin, question);
            stringstream ss(question);
            string name1, name2, relation;
            string is, a, of;

            ss >> name1 >> is >> a >> relation >> of >> name2;
            if (!name2.empty() && name2.back() == '.')
            {
                name2.pop_back();
            }

            if (family_tree.find(name1) == family_tree.end() || family_tree.find(name2) == family_tree.end())
            {
                cout << "False" << endl;
                continue;
            }

            // 查找该key值（名字）对应的父结点指针
            TreeNode *node1 = family_tree[name1];
            TreeNode *node2 = family_tree[name2];

            if (relation == "parent")
            {
                if (node2->parent == node1)
                    cout << "True" << endl;
                else
                    cout << "False" << endl;
            }
            else if (relation == "child")
            {
                if (node1->parent == node2)
                    cout << "True" << endl;
                else
                    cout << "False" << endl;
            }
            else if (relation == "sibling")
            {
                if (node1 == node2)
                {
                    cout << "True" << endl;
                }
                else if (node1->parent != nullptr && node2->parent != nullptr && node1->parent == node2->parent)
                    cout << "True" << endl;
                else
                    cout << "False" << endl;
            }
            else if (relation == "descendant")
            {
                if (node1 == node2)
                {
                    cout << "True" << endl;
                }
                else
                {
                    bool ok = false;
                    TreeNode *curr = node1->parent;
                    while (curr != nullptr)
                    {
                        if (curr == node2)
                        {
                            ok = true;
                            break;
                        }
                        curr = curr->parent;
                    }
                    if (ok)
                        cout << "True" << endl;
                    else
                        cout << "False" << endl;
                }
            }
            else if (relation == "ancestor")
            {
                if (node1 == node2)
                {
                    cout << "True" << endl;
                }
                else
                {
                    bool ok = false;
                    TreeNode *curr = node2->parent;
                    while (curr != nullptr)
                    {
                        if (curr == node1)
                        {
                            ok = true;
                            break;
                        }
                        curr = curr->parent;
                    }
                    if (ok)
                        cout << "True" << endl;
                    else
                        cout << "False" << endl;
                }
            }
            else
            {
                cout << "False" << endl;
            }
        }
        cout << endl;
    }
    return 0;
}
