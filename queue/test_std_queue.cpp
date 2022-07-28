#include<queue>
#include<iostream>
using namespace std;

void test_1(){
    queue<int> q1{{1,2,3,4,5}};
    cout<<q1.front()<<"  "<<q1.back()<<'\n';
}

int main(){

    system("pause");
    return 0;
}
