#include<iostream>
#include<deque>
#include"../deque/deque.hpp"
using namespace std;

template<typename T>
using My_Deque=MySTL::deque<T>;

void test_1(){
    bool flag=true;
    My_Deque<int> m1{};
    deque<int> d1{};
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    for(int i=0;i<100;++i){
        if(i%2){
            m1.push_back(i);
            d1.push_back(i);
        }
        else {
            m1.push_front(i);
            d1.push_front(i);
        }
    }
    // 注意auto的使用
    auto m_itr=m1.begin();
    auto itr=d1.begin();
    for(int i=0;i<m1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"push wrong\n";
    }
    for(int i=100;i<=2000;++i){
        if(i%3){
            m1.push_back(i);
            d1.push_back(i);
        }
        else {
            m1.push_front(i);
            d1.push_front(i);
        }
    }
    m_itr=m1.begin(),itr=d1.begin();
    for(int i=0;i<d1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"push again wrong\n";
    }
    for(int i=0;i<1000;++i){
        if(i%2) {
            m1.pop_back();
            d1.pop_back();
        }
        else{
            m1.pop_front();
            d1.pop_front();
        }
    }
    m_itr=m1.begin(),itr=d1.begin();
    for(int i=0;i<m1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"pop wrong\n";
    }
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    auto m_itr_2=m1.begin()+(m1.size()>>1);
    auto itr_2=d1.begin()+(d1.size()>>1);
    auto m_itr_3=m1.erase(m_itr_2);auto itr_3=d1.erase(itr_2);
    if(*m_itr_3!=*itr_3) cout<<"erase single returns wrong\n";
    m_itr=m1.begin(),itr=d1.begin();
    for(int i=0;i<m1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"erase single wrong\n";
    }
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin()+(m1.size()>>2);itr=d1.begin()+(d1.size()>>2);
    m_itr=m1.erase(m_itr,m_itr_3);itr=d1.erase(itr,itr_3);
    if(*m_itr!=*itr) cout<<"erase [b,e) wrong\n";
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"erase [b,e) wrong\n";
    }
    cout<<"begin traverse\n";
    cout<<"My_Deque:\t\tstd::deque:\n";
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        cout<<*m_itr<<"\t\t\t"<<*itr<<'\n';
    }
    cout<<"end traverse\n";
    m1.assign(10,10);d1.assign(10,10);
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,m_itr++,itr++){
        if(*m_itr!=*itr) std::cout<<"assign n vals wrong\n";
    }
    m1.clear();d1.clear();
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m1={1,2,3,4,5,6,7,8,9,10};d1={1,2,3,4,5,6,7,8,9,10};
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        if(*m_itr!=*itr) std::cout<<"={elems} wrong\n";
    }
    m_itr_2=m1.begin()+(m1.size()>>1);itr_2=d1.begin()+(d1.size()>>1);
    cout<<(m_itr_2-m1.begin())<<"   "<<(itr_2-d1.begin())<<'\n';
    if((m_itr_2-m1.begin())!=(itr_2-d1.begin())) cout<<"minus between iterators wrong\n";
    m_itr=m1.insert(m_itr_2,11);itr=d1.insert(itr_2,11);
    if(*m_itr!=*itr) cout<<"insert single returns wrong\n";
    cout<<*m_itr<<"   "<<*itr<<'\n';
    cout<<*(m_itr+1)<<"   "<<*(itr+1)<<'\n';
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        if(*m_itr!=*itr) std::cout<<"insert single wrong\n";
    }
    cout<<"begin traverse\n";
    cout<<"My_Deque:\t\tstd::deque:\n";
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        cout<<*m_itr<<"\t\t\t"<<*itr<<'\n';
    }
    cout<<"end traverse\n";
    m_itr_2=m1.begin()+(m1.size()>>2);itr_2=d1.begin()+(d1.size()>>2);
    cout<<(m_itr_2-m1.begin())<<"   "<<(itr_2-d1.begin())<<'\n';
    if((m_itr_2-m1.begin())!=(itr_2-d1.begin())) cout<<"minus between iterators wrong\n";
    m_itr=m1.insert(m_itr_2,11);itr=d1.insert(itr_2,11);
    if(*m_itr!=*itr) cout<<"insert single returns wrong\n";
    cout<<*m_itr<<"   "<<*itr<<'\n';
    cout<<*(m_itr+1)<<"   "<<*(itr+1)<<'\n';
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        if(*m_itr!=*itr) std::cout<<"insert single wrong\n";
    }
    cout<<"begin traverse\n";
    cout<<"My_Deque:\t\tstd::deque:\n";
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        cout<<*m_itr<<"\t\t\t"<<*itr<<'\n';
    }
    cout<<"end traverse\n";

    m_itr=m1.begin()+(m1.size()>>1);itr=d1.begin()+(m1.size()>>1);
    m_itr=m1.insert(m_itr,3,12);itr=d1.insert(itr,3,12);

    if(*m_itr!=*itr) cout<<"insert n vals returns wrong\n";
    cout<<*m_itr<<"   "<<*itr<<'\n';
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        if(*m_itr!=*itr) cout<<"insert n vals wrong\n";
    }

    cout<<"begin traverse\n";
    cout<<"My_Deque:\t\tstd::deque:\n";
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        cout<<*m_itr<<"\t\t\t"<<*itr<<'\n';
    }
    cout<<"end traverse\n";

    m_itr=m1.begin()+(m1.size()>>2);itr=d1.begin()+(m1.size()>>2);
    m_itr=m1.insert(m_itr,2000,12);itr=d1.insert(itr,2000,12);

    if(*m_itr!=*itr) cout<<"insert n vals returns wrong\n";
    cout<<*m_itr<<"   "<<*itr<<'\n';
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<m1.size();++i,++m_itr,++itr){
        if(*m_itr!=*itr) cout<<"insert n vals wrong\n";
    }
    // so big do not traverse


    My_Deque<int> m2{m1.begin()+3,m1.begin()+10};
    deque<int> d2{d1.begin()+3,d1.begin()+10};
    cout<<m2.size()<<"   "<<d2.size()<<'\n';
    m_itr=m2.begin();itr=d2.begin();
    for(int i=0;i<d2.size();++i,++m_itr,++itr){
        if(*itr!=*m_itr) cout<<"[b,e) constructor went wrong\n";
    }
    m2.swap(m1);d2.swap(d1);
    cout<<m2.size()<<"   "<<d2.size()<<'\n';
    m_itr=m2.begin();itr=d2.begin();
    for(int i=0;i<d2.size();++i,++m_itr,++itr){
        if(*itr!=*m_itr) cout<<"swap went wrong\n";
    }
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<d1.size();++i,++m_itr,++itr){
        if(*itr!=*m_itr) cout<<"swap went wrong\n";
    }
    swap(m1,m2);swap(d1,d2);
    cout<<m2.size()<<"   "<<d2.size()<<'\n';
    m_itr=m2.begin();itr=d2.begin();
    for(int i=0;i<d2.size();++i,++m_itr,++itr){
        if(*itr!=*m_itr) cout<<"swap went wrong\n";
    }
    cout<<m1.size()<<"   "<<d1.size()<<'\n';
    m_itr=m1.begin();itr=d1.begin();
    for(int i=0;i<d1.size();++i,++m_itr,++itr){
        if(*itr!=*m_itr) cout<<"swap went wrong\n";
    }

    if((m1.front()!=d1.front())||(m2.front()!=d2.front())){
        cout<<"something goes wrong with front()\n";
    }
    if((m1.back()!=d1.back())||(m2.back()!=d2.back())){
        cout<<"something goes wrong with back()\n";
    }

    for(int i=0;i<m1.size();++i){
        if(m1[i]!=d1[i]) cout<<"something was wrong within [] function\n";
    }

    for(int i=0;i<m2.size();++i){
        if(m2[i]!=d2[i]) cout<<"something was wrong within [] function\n";
    }

    try{
        for(int i=0;i<=m1.size();++i){
            if(m1.at(i)!=d1.at(i)) cout<<"something was wrong within at function\n";
        }
    }
    catch(...){
        cout<<"at throw successfully\n";
    }

    try{
        for(int i=0;i<=m2.size();++i){
            if(m2.at(i)!=d2.at(i)) cout<<"something was wrong within [] function\n";
        }
    }
    catch(...){
        cout<<"at throw successfully\n";
    }


}

// void test_2(){
//     My_Deque<int> m1{1,2,11,3,4,5,11,6,7,8,9,10};
//     deque<int> d1{1,2,11,3,4,5,11,6,7,8,9,10};
//     auto m_itr=m1.begin()+(m1.size()>>1);auto itr=d1.begin()+(m1.size()>>1);
//     m_itr=m1.insert(m_itr,3,12);itr=d1.insert(itr,3,12);
// }

int main(){
    test_1();
    // if no error message occur in the shell , then the deque is perfect

    // test_2();
    system("pause");
    return 0;
}
