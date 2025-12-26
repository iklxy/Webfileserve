#include <iostream>
using namespace std;

int josephus(int n, int m) {
    int result = 0; 
    
    
    for (int i = 2; i <= n; i++) 
    {
        result = (result + m) % i;
    }
    
    return result;
}

int main() {
    int n, k;
    cin >> n >> k;
    cout << josephus(n, k) << endl;
    return 0;
}