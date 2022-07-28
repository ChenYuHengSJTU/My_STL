#include<stack>
#include<iostream>
using namespace std;

void test_throw(){
    stack<double> s1{};
    auto p=s1.top(); // no throw
}

int main(){
    test_throw();
    system("pause");
    return 0;
}
