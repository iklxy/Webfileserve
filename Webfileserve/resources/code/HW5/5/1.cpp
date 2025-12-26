#include <iostream>
#include <vector>
using namespace std;
typedef unsigned long long int ull;

bool is_prime(int n)
{
    if (n <= 1)
        return false;
    if (n == 2 || n == 3)
        return true;
    if (n % 2 == 0)
        return false;
    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int get_next_prime(int n)
{
    while (!is_prime(n))
        n++;
    return n;
}

int get_key(string name, int P)
{
    ull key = 0;
    for (char c : name)
    {
        key = key * 37 + c;
    }
    return key % P;
}

int main()
{
    int N, M;
    cin >> N >> M;
    int P = get_next_prime(M);

    // 初始化哈希表
    vector<string> hash_table(P, "");

    for (int i = 0; i < N; i++)
    {
        string name;
        cin >> name;

        // 计算哈希值
        int key = get_key(name, P);
        bool found = false;
        int final_pos = -1;

        // 探测次数不应超过表长P
        for (int j = 0; j <= P; j++)
        {
            int index;
            int k = (j + 1) / 2;
            long long offset = (long long)k * k;

            if (j == 0)
            {
                index = key;
            }
            else
            {
                if (j % 2 == 1)
                {
                    // 奇数次
                    index = (key + offset) % P;
                }
                else
                {
                    index = ((key - offset) % P + P) % P;
                }
            }

            if (hash_table[index] == "")
            {
                hash_table[index] = name;
                final_pos = index;
                found = true;
                break;
            }

            // 如果探测次数达到表长，停止
            if (j >= P)
                break;
        }

        if (i > 0)
            cout << " ";

        if (found)
            cout << final_pos;
        else
            cout << "-";
    }

    return 0;
}