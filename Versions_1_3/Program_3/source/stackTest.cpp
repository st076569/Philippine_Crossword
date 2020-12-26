#include <stack>
#include <iostream>

int main()
{
    std::stack<int> s1;
    int        temp;
    
    s1.push(5);
    s1.push(7);
    s1.push(55);
    temp = s1.top();
    s1.pop();
    
    std::cout << temp;
    
    return 0;
}