#include<iostream>
#include<queue>
#include"queue.hpp"
using namespace std;

template<typename T,typename Sequence=MySTL::deque<T>>
using MyQueue=MySTL::queue<T,Sequence>;


void test_1(){
    bool flag=true;
    queue<int> q1{};
    MyQueue<int> m1{};
    cout<<q1.size()<<"   "<<m1.size()<<'\n';
    for(int i=1;i<=100000;++i){
        if(i%3){
            q1.push(i);
            m1.push(i);
            if((q1.back()!=m1.back())||(q1.front()!=m1.front())){
                cout<<"something wrong occurs in push , back or front\n";
                flag=false;
            }
        }
        else {
            q1.pop();
            m1.pop();
            if((q1.back()!=m1.back())||(q1.front()!=m1.front())){
                cout<<"something wrong occurs in push , back or front\n";
                flag=false;
            }
        }
    }
    cout<<q1.size()<<"   "<<m1.size()<<'\n';
    int q,m;

    MyQueue<int> m2{m1};
    queue<int> q2{q1};
    cout<<"MuQueue:\tstd::queue:\n";
    cout<<m2.size()<<"   "<<q2.size()<<'\n';

    while((!q2.empty())&&(!m2.empty())){
        q=q2.back();
        m=m2.back();
        if(q!=m) {
            cout<<"copy and back wrong\n";
            flag=false;
        }
        q=q2.front();
        m=m2.front();
        if(q!=m) {
            cout<<"copy and front wrong\n";
            flag=false;
        }
        cout<<m<<"\t\t"<<q<<"\n";
        q2.pop();
        m2.pop();
    }

    while((!q1.empty())&&(!m1.empty())){
        q=q1.front();
        m=m1.front();
        if(q!=m) {
            cout<<"something wrong occurs in push , back or front\n";
            flag=false;
        }
        m1.pop();
        q1.pop();
    }

    cout<<q1.size()<<"   "<<m1.size()<<'\n';
    if(!flag) cout<<"test_1 failed......\n";
}

void test_move(){
    queue<int> q1{{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}};
    MyQueue<int> m1{{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}};
    cout<<q1.size()<<"   "<<m1.size()<<'\n';
    int q,m;
    while((!q1.empty())&&(!m1.empty())){
        q=q1.front();
        m=m1.front();
        if(q!=m) cout<<"something wrong occurs in push , back or front\n";
        m1.pop();
        q1.pop();
    }

    cout<<q1.size()<<"   "<<m1.size()<<'\n';
}

void test_swap(){
    bool flag=true;
    queue<int> q1{};
    MyQueue<int> m1{};
    cout<<q1.size()<<"   "<<m1.size()<<'\n';
    for(int i=1;i<=100000;++i){
        if(i%3){
            q1.push(i);
            m1.push(i);
            if((q1.back()!=m1.back())||(q1.front()!=m1.front())){
                cout<<"something wrong occurs in push , back or front\n";
                flag=false;
            }
        }
        else {
            q1.pop();
            m1.pop();
            if((q1.back()!=m1.back())||(q1.front()!=m1.front())){
                cout<<"something wrong occurs in push , back or front\n";
                flag=false;
            }
        }
    }
    cout<<q1.size()<<"   "<<m1.size()<<'\n';


    queue<int> q2{{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}};
    MyQueue<int> m2{{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}};
    cout<<q1.size()<<"   "<<m1.size()<<'\n';


    int q,m;

    m2.swap(m1);
    q2.swap(q1);

    while((!q2.empty())&&(!m2.empty())){
        q=q2.back();
        m=m2.back();
        if(q!=m) {
            cout<<"copy and back wrong\n";
            flag=false;
        }
        q=q2.front();
        m=m2.front();
        if(q!=m) {
            cout<<"copy and front wrong\n";
            flag=false;
        }
        q2.pop();
        m2.pop();
    }

    while((!q1.empty())&&(!m1.empty())){
        q=q1.back();
        m=m1.back();
        if(q!=m) {
            cout<<"copy and back wrong\n";
            flag=false;
        }
        q=q1.front();
        m=m1.front();
        if(q!=m) {
            cout<<"copy and front wrong\n";
            flag=false;
        }
        q1.pop();
        m1.pop();
    }

    if(!flag) cout<<"test swap failed......\n";
}

int main(){
    // test_1();
    // test_move();
    // test_swap();
    system("pause");
    return 0;
}
