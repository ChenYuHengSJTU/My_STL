#include<stack>
#include"stack.hpp"
#include<iostream>
using namespace std;

template<typename T,typename Sequence=MySTL::deque<T>>
using My_stack=MySTL::stack<T,Sequence>;

void test_with_std_stack_1(){
    stack<int> s1{};
    My_stack<int> m1{};
    cout<<m1.size()<<"   "<<s1.size()<<'\n';
    int s,m;
    for(int i=1;i<=20000;++i){
        try{
            if(!(i%3)) {
                s=s1.top(); // std::stack未做当stack为空时的throw异常处理
                m=m1.top(); // My_Stack做了stack为空是的异常抛出处理；
                s1.pop();
                m1.pop();
                if(s!=m) cout<<"top wrong\n";
                if(s1.top()!=m1.top()) cout<<"pop wrong\n";
            }
            else {
                s1.push(i);
                m1.push(i);
                if(s1.top()!=m1.top()) cout<<"push wrong\n";
            }
        }
        catch(typename My_stack<int>::stack_is_empty){
            cout<<"you use top or pop wrongly while the stack is empty\n";
            cout<<m1.size()<<"   "<<s1.size()<<'\n';
        }
        catch(...) {
            cout<<"somewhere else occurs wrong\n";
            cout<<m1.size()<<"   "<<s1.size()<<'\n';
        }
    }
    cout<<m1.size()<<"   "<<s1.size()<<'\n';
    cout<<"My_Stack:\tstd::stack\n";
    while((!m1.empty())&&(!s1.empty())){
        m=m1.top();
        s=s1.top();
        m1.pop();
        s1.pop();
        if(s!=m) cout<<"WRONG!!!\n";
        cout<<m<<"\t\t"<<m<<'\n';
    }
}

void test_swap(){
    stack<int> s1{};
    My_stack<int> m1{};
    int s,m;
    for(int i=1;i<=20000;++i){
        if(!(i%3)) {
            s=s1.top();
            m=m1.top();
            s1.pop();
            m1.pop();
            if(s!=m) cout<<"top wrong\n";
            if(s1.top()!=m1.top()) cout<<"pop wrong\n";
        }
        else {
            s1.push(i);
            m1.push(i);
            if(s1.top()!=m1.top()) cout<<"push wrong\n";
        }
    }
    My_stack<int> m2{{1,2,3,4,5}};
    stack<int> s2{{1,2,3,4,5}};
    cout<<m1.size()<<"   "<<s1.size()<<'\n';
    cout<<m2.size()<<"   "<<s2.size()<<'\n';
    s2.swap(s1);m1.swap(m2);
    cout<<m1.size()<<"   "<<s1.size()<<'\n';
    cout<<m2.size()<<"   "<<s2.size()<<'\n';

    while((!m1.empty())&&(!s1.empty())){
        m=m1.top();
        s=s1.top();
        m1.pop();
        s1.pop();
        if(s!=m) cout<<"WRONG!!!\n";
        // cout<<m<<"\t\t"<<m<<'\n';
    }

    while((!m2.empty())&&(!s2.empty())){
        m=m2.top();
        s=s2.top();
        m2.pop();
        s2.pop();
        if(s!=m) cout<<"WRONG!!!\n";
        // cout<<m<<"\t\t"<<m<<'\n';
    }
    cout<<"swap is ok!\n";
}

void test_copy(){
    stack<int> s1{};
    My_stack<int> m1{};
    int s,m;
    for(int i=1;i<=20000;++i){
        if(!(i%3)) {
            s=s1.top();
            m=m1.top();
            s1.pop();
            m1.pop();
            if(s!=m) cout<<"top wrong\n";
            if(s1.top()!=m1.top()) cout<<"pop wrong\n";
        }
        else {
            s1.push(i);
            m1.push(i);
            if(s1.top()!=m1.top()) cout<<"push wrong\n";
        }
    }
    My_stack<int> m2{{1,2,3,4,5}};
    stack<int> s2{{1,2,3,4,5}};
    My_stack<int> m3{m1};
    while((!m1.empty())&&(!m3.empty())){
        auto tmp_m=m3.top();m3.pop();
        m=m1.top();m1.pop();
        if(tmp_m!=m){
            cout<<"copy wrong\n";
            cout<<tmp_m<<"  "<<m<<'\n';
        }
    }
    cout<<m1.size()<<"   "<<m3.size()<<'\n';
}

int main(){
    // test_with_std_stack_1();
    // test_swap();
    test_copy();
    system("pause");
    return 0;
}
