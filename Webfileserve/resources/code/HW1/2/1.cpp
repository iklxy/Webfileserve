#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
//定义一个名为Student的结构体，里面存储着no，name两个数据，no代表student的学号，name代表student的名字
struct Student 
{
    string no;//学号
    string name;//名字
};

int main() {
    int n;
    cin >> n;
    cin.ignore(); // 跳过换行
    vector<Student> stu;//使用vector动态数组来存储 stu中的每一个元素是一个Student对象
    for(int i=0; i<n; ++i) 
    {
        Student s;
        cin >> s.no>>s.name;
        stu.push_back(s);
    }
    string input;
    while(getline(cin,input) || input=="end") 
    {
        istringstream iss(input);
        string cmd;
        iss>>cmd;
        if(cmd == "end") 
        {
            cout << stu.size() << endl;
            break;
        }
        if(cmd == "insert") 
        {
            int pos;
            string no, name;
            iss >> pos >> no >> name;
            if(pos < 1 || pos > stu.size()+1) 
            {
                cout << -1 << endl;
            } 
            else 
            {
                stu.insert(stu.begin()+pos-1, {no, name});
                cout << 0 << endl;
            }
        } 
        else if(cmd == "remove") 
        {
            int pos;
            iss >> pos;
            if(pos < 1 || pos > stu.size()) 
            {
                cout << -1 << endl;
            } 
            else 
            {
                stu.erase(stu.begin()+pos-1);
                cout << 0 << endl;
            }
        } 
        else if(cmd == "check") 
        {
            string type, value;
            iss>>type>>value;
            if(type == "name") 
            {
                int found = -1;
                for(int i=0; i<stu.size(); ++i) 
                {
                    if(stu[i].name == value) 
                    {
                        cout << i+1 << " " << stu[i].no << " " << stu[i].name << endl;
                        found = 1;
                        break;
                    }
                }
                if(found == -1) cout << -1 << endl;
            }
             else if(type == "no") 
             {
                int found = -1;
                for(int i=0; i<stu.size(); ++i) 
                {
                    if(stu[i].no == value) 
                    {
                        cout << i+1 << " " << stu[i].no << " " << stu[i].name << endl;
                        found = 1;
                        break;
                    }
                }
                if(found == -1) cout << -1 << endl;
            }
        }
    }
    return 0;
}