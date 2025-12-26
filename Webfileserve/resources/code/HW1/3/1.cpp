#include<iostream>
#include<string>
#include<map>
#include<sstream>
using namespace std;
/*定义一个名为Node的结构体，一个Node代表多项式中的一个单项式，其中pair对存储单项式的底数和指数，first是底数，
second是指数,next是指向下一个单项式的指针*/
struct Node
{
    pair<int,int>data;//存储单项式的底数和指数
    Node* next;//指向下一个单项式的指针
    Node(pair<int,int> val)//Node的构造函数
    {
        data=val;
        next=nullptr;
    };
};

Node* Add(Node* l1,Node* l2)
{
    Node* p1=l1;
    Node* p2=l2;
    Node* sum=new Node({0,0});
    Node* temp=sum;
    while(p1 && p2)
    {
        if(p1->data.second>p2->data.second)
        {
            temp->next=new Node(p2->data);
            p2=p2->next;
            temp=temp->next;
        }
        else if(p1->data.second<p2->data.second)
        {
            temp->next=new Node(p1->data);
            p1=p1->next;
            temp=temp->next;
        }
        else
        {
            if(p1->data.first+p2->data.first==0)
            {
                p1=p1->next;
                p2=p2->next;
                continue;
            }
            temp->next=new Node({p1->data.first+p2->data.first,p1->data.second});
            temp=temp->next;
            p1=p1->next;
            p2=p2->next;
        }
    }
    while(p1)
    {
        temp->next=new Node(p1->data);
        p1=p1->next;
        temp=temp->next;
    }
    while(p2)
    {
        temp->next=new Node(p2->data);
        p2=p2->next;
        temp=temp->next;
    }
    return sum->next;
}

Node* mul(Node* l1,Node* l2)
{
    map<int,int>terms;
    for(Node* p1=l1;p1!=nullptr;p1=p1->next)
    {
        for(Node* p2=l2;p2!=nullptr;p2=p2->next)
        {
            int coef = p1->data.first * p2->data.first;
            int exp = p1->data.second + p2->data.second;
            terms[exp] += coef;
        }
    }
    Node* product=new Node({0,0});
    Node* temp=product;
    for(auto it=terms.begin();it!=terms.end();it++)
    {
        temp->next=new Node(*it);
        temp=temp->next;
    }
    return product->next;
}

int main()
{
    int first;
    cin>>first;
    cin.ignore();
    string str;
    getline(cin,str);
    istringstream iss(str);
    int key,value;
    Node* head = nullptr;
    Node* temp=head;
    while(iss>>key>>value)
    {
        Node* newNode=new Node({key,value});
        if (!head) 
        {
            head = newNode;
            temp = head;
        } 
        else 
        {
            temp->next = newNode;
            temp = newNode;
        }
    }

    int second;
    cin>>second;
    cin.ignore();
    string str2;
    getline(cin,str2);
    istringstream iss2(str2);
    int key2,value2;
    Node* head2 = nullptr;
    Node* temp2=head2;
    while(iss2>>key2>>value2)
    {
        Node* newNode=new Node({key2,value2});
        if (!head2) 
        {
            head2 = newNode;
            temp2 = head2;
        } else 
        {
            temp2->next = newNode;
            temp2 = newNode;
        }
    }
    int choose;
    cin>>choose;
    if(choose==0)
    {
        Node* sum=nullptr;
        sum=Add(head,head2);
        for(Node* cur=sum;cur!=nullptr;cur=cur->next)
        {
        cout<<cur->data.first<<" "<<cur->data.second<<" ";
        }
        cout<<endl;
    }
    if(choose==1)
    {
        Node* product=nullptr;
        product=mul(head,head2);
        for(Node* cur=product;cur!=nullptr;cur=cur->next)
        {
        cout<<cur->data.second<<" "<<cur->data.first<<" ";
        }
        cout<<endl;
    }
    if(choose==2)
    {
        Node* sum=nullptr;
        sum=Add(head,head2);
        for(Node* cur=sum;cur!=nullptr;cur=cur->next)
        {
        cout<<cur->data.first<<" "<<cur->data.second<<" ";
        }
        cout<<endl;
        Node* product=nullptr;
        product=mul(head,head2);
        for(Node* cur=product;cur!=nullptr;cur=cur->next)
        {
        cout<<cur->data.second<<" "<<cur->data.first<<" ";
        }
        cout<<endl;
    }
    return 0;
}