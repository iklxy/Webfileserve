/*队列中的最大值*/
#include<iostream>
#include<vector>
#include<deque>
#include <sstream>
using namespace std;

//max_queue class
class max_queue
{
    private:
    vector<int>Queue;
    int front,rear;//front指向队列的头部，rear指向队列的尾部的下一个位置
    int size,capacity;
    deque<int>Max_queue;//用来存储队列中的最大值
    public:

    max_queue(int cap):capacity(cap),front(0),rear(0),size(0)
    {
        Queue.resize(capacity);
    };

    void Enqueue(int m)
    {
        //Enqueue
        if(Is_full())
        {
            cout<<"Queue is Full"<<endl;
            return;
        }
        Queue[rear]=m;
        rear=(rear+1) % capacity;
        size++;
        //Update Max_queue
        while(!Max_queue.empty() && Max_queue.back()<m)//Max_queue.back() is the last element in Max_queue
        {
            Max_queue.pop_back();
        }
        Max_queue.push_back(m);
        return;
    }

    void Dequeue()
    {
        if(Is_empty())
        {
            cout<<"Queue is Empty"<<endl;
            return;
        }
        int temp=Queue[front];
        front=(front+1) % capacity;
        size--;
        cout<<temp<<endl;
        //Update Max_queue
        if(temp == Max_queue.front())//Max_queue.front() is the first element in Max_queue
        {
            Max_queue.pop_front();
        }
        return;
    }

    void GetMax()
    {
        if(Is_empty())
        {
            cout<<"Queue is Empty"<<endl;
            return;
        }
        cout<<Max_queue.front()<<endl;
        return;
    }

    void quit()
    {
      while(size!=0)
      {
        cout<<Queue[front]<<" ";
        front=(front+1) % capacity;
        size--;
      }
        cout<<endl;
    }

    bool Is_empty()
    {
        return size==0;
    }

    bool Is_full()
    {
        return size==capacity;
    }
};

int main()
{
    int n;
    cin>>n;
    cin.ignore(1024,'\n');
    max_queue mq(n);
    //Command
    string cmd[]={"enqueue","dequeue","max","quit"};
    
    string operation;
    while(getline(cin,operation))
    {
        istringstream iss(operation);
        string command;
        int val;
        iss>>command;
        if(command==cmd[0])//Enqueue
        {
            iss>>val;
            mq.Enqueue(val);
        }
        else if(command==cmd[1])//Dequeue
        {
            mq.Dequeue();
        }
        else if(command==cmd[2])//GetMax
        {
            mq.GetMax();
        }
        else if(command==cmd[3])//quit
        {
            mq.quit();
            break;
        }
    }
}