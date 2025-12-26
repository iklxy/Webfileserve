#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<sstream>
using namespace std;

//卡牌类 用来表示单张卡牌 其中存储的三个成员分别代表 排的大小（rank） 牌的种类（suit） 指向下一张牌的指针（next）
class Card
{
    public:
    string rank;//牌的大小
    string suit;//牌的种类
    Card* next;//指向下一张牌的指针
    //Card的构造函数
    Card(string s,string r):suit(s),rank(r),next(nullptr){};
    Card(){};
};

/*Desk类 代表操作对卡牌的操作 其中private中的成员变量head代表牌堆的顶部
其中有Append Extract Pop Revert四个函数 还有一个gethead函数用来获取牌堆的顶部 便于操作*/
class Desk
{
    private:
    Card* head;//牌堆的顶部

    int getRankValue(string rank) //把AJQK四个牌的点数改写为数字
    {
        if (rank == "A") return 1;
        if (rank == "J") return 11;
        if (rank == "Q") return 12;
        if (rank == "K") return 13;
        return stoi(rank);
    }
    public:
    Desk()//Desk的构造函数
    {
        head=nullptr;
    };
    ~Desk()//Desk的析构函数
    {
        while(head)
        {
            Card* temp=head;
            head=temp->next;
            delete temp;
        }
    }
    //Append
    void Append(string suit,string rank)
    {
        if(!head)
        {
            head=new Card(suit,rank);
            return;
        }
        //找到牌堆的最后一张牌 插入在牌堆底部
        Card* cur=head;
        while(cur->next)
        {
            cur=cur->next;
        }
        cur->next=new Card(suit,rank);
    }
    //Extract
    void Extract(string s)
    {
        //用vector来存储符合suit种类的牌的节点，因为考虑到使用容器的话能够使用STL中封装好的sort函数来进行排序
        vector<Card*> seen;
        Card* prev=nullptr;
        Card* cur=head;
        while(cur)
        {
            if(cur->suit==s)
            {
                seen.push_back(cur);
                if(prev)
                {
                   prev->next=cur->next;
                   Card* temp = cur;
                   cur=prev->next;
                   temp->next=nullptr;
                }
                else
                {
                    head=cur->next;
                    Card* temp = cur;
                    cur=head;
                    temp->next=nullptr;
                }
            }
            else
            {
                prev=cur;
                cur=cur->next;
            }
        }
        if(seen.empty())return;
        //sort
        sort(seen.begin(), seen.end(), [this](Card* a, Card* b) 
        {
            return getRankValue(a->rank) <= getRankValue(b->rank);
        });
        //使用反向迭代器 因为题目要求由小到大插入到牌堆顶部 rebegin迭代器指向seen中rank值最大的一个元素
        for (auto it = seen.rbegin(); it != seen.rend(); ++it) 
        {
            (*it)->next = head;
            head = *it;
        }
    }
    //Pop
    void Pop()
    {
        if(!head)
        {
            cout<<"NULL"<<endl;
            return;
        }
        cout<<head->suit<<" "<<head->rank<<endl;
        Card* temp=head;
        head=head->next;
        delete temp;
    }
    //Revert
    void Revert()
    {
        Card* prev=nullptr;
        Card* cur=head;
        //双指针法来完成链表的反转 cur指向prev 每一次移动 cur都重新赋值为cur->next 
        while(cur)
        {
            Card* next=cur->next;
            cur->next=prev;
            prev=cur;
            cur=next;
        }
        head=prev;
    }
    //gethead
    Card* gethead()
    {
        return head;
    }
};

int main()
{
    Desk desk;
    //the numer of operation
    string n_str;
    getline(cin, n_str);
    int n = stoi(n_str);
    string operation;

    while(n--)
    {
        getline(cin,operation);
        if(operation.empty()) 
        {
            n++;  // 不计数，重新读取
            continue;
        }
        string cmd,suit,rank;
        istringstream iss(operation);
        iss>>cmd;
        if(cmd=="Append")
        {
            iss>>suit>>rank;
            desk.Append(suit,rank);
        }
        else if(cmd=="Extract")
        {
            iss>>suit;
            desk.Extract(suit);
        }
        else if(cmd=="Pop")
        {
            desk.Pop();
        }
        else if(cmd=="Revert")
        {
            desk.Revert();
        }
    }
    Card* temp=desk.gethead();
    if(!temp)
    {
        cout<<"NULL"<<endl;
    }
    else
    {
        while(temp)
        {
            cout<<temp->suit<<" "<<temp->rank<<endl;
            temp=temp->next;
        }
    }
    return 0;
}