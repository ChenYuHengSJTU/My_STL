#ifndef MySTL_Deque         // 包含保护
#define MySTL_Deque

// Created By ChenYuheng from Jun 24 2022 to Jun 26 2022

#include<cstddef>          // 定义了size_t
#include<memory>           // 定义了allocator delete和new
#include<initializer_list> // 使用初始化器列表进行初始化
#include<string>           // 在异常类中使用string成员记录异常信息
#include<iostream>         //
#include<exception>       // 构造继承类exceptions继承自std::exception
#include<cassert>         // 使用断言 编译时断言或者运行时断言
#include<algorithm>

using std::allocator;

namespace MySTL {
    template<typename T,typename Alloc=std::allocator<T>,size_t BufferSize=0>
    class deque {
        // 注意deque与vector最大的区别 ： 可以在常量时间内完成其头部的操作 同时，deque没有所谓“容量”的概念
        // 注意一个大局观 所有用来初始化类型形参T的类必须具有默认构造函数 因此map为value_type**
        // 如果需要类型实参的多态性 请使用类型实参的指针来初始化类型形参

        // 一系列using的含义是使得在不同容器中具有不同含义的类型具有一个统一的名字 相当于C语言中的typedef
        using value_type=T;
        using size_type=size_t;
        using reference_type=T&;
        using const_reference_type=const T&;
        using pointer=T*;
        using const_pointer=const T*;
        using allocator_type=Alloc;
        using difference_type=ptrdiff_t;
        using map_pointer=value_type**;

        //默认map的大小
        const size_type INITIAL_SIZE=8;

        public:

        class const_iterator;
        // 两个迭代器，都为随机访问迭代器
        class iterator{
            // 还有一些修改未同步到const_iterator
            friend class deque;   // 定义友元 如果不定义友元 需要定义get--系列函数(注意返回引用)
            friend class const_iterator;
            private:
            // 一定要注意变量声明的顺序(即初始化的顺序)
            // 类内初始化器
            deque* d=nullptr;
            value_type** map_pointer=nullptr; //中控区
            size_type Bufsize=0;  // 缓冲区大小
            size_type index=0;    // 缓冲区编号 利于实现
            pointer cur=nullptr,first=nullptr,finish=nullptr;  // 比vector之流的迭代器复杂所在 需要利用cur，first，finish之间的关系进行判断

            // 拷贝 工具函数 将实现定义为private 接口为public；
            inline void copy(const iterator& other){
                d=other.d;
                cur=other.cur;
                first=other.first;
                finish=other.finish;
                Bufsize=other.Bufsize;
                index=other.index;
                map_pointer=other.map_pointer;
            }

            // iterator 是否需要移动构造函数？
            inline void move(iterator& other){
                d=other.d;
                cur=other.cur;
                first=other.first;
                finish=other.finish;
                Bufsize=other.Bufsize;
                index=other.index;
                map_pointer=other.map_pointer;
                other.cur=nullptr;
            }
            public:
            // 定义自己使用的异常类
            class wrong_iterator : public std::exception{
                public:
                std::string message="the two iterator does not refer to the same deque\n";
                // override基类中的what()
                // 要加throw()???
                inline const char* what() const throw() override {return message.c_str();}
            };

            // 一些构造函数
            iterator(deque* _d,size_type i=0,size_type j=0):d{_d},Bufsize{_d->Bufsize},map_pointer{_d->map},
                                                            index{i},cur{map_pointer[i]+j},first{map_pointer[i]},
                                                            finish{map_pointer[i]+Bufsize-1}{
            }

            iterator(const iterator& other){
                copy(other);
            }

            iterator(iterator && other){
                move(other);
            }

            iterator(){}

            ~iterator(){}

            inline iterator& operator=(const iterator& other){
                copy(other);
                return *this;
            }

            inline iterator& operator+=(const size_type& n){
                // 一般重载迭代器都是先实现成员函数+= 然后利用+=实现其余的运算符；但是deque中考虑到+=函数过于庞大，调用代价过高
                // 故更加常用的 ++ 单独实现；
                size_type now=Bufsize*index+size_type(cur-first);
                size_type res=now+n;
                index=res/Bufsize;
                size_type _index=res%Bufsize;
                if(index>=d->map_size&&_index) throw index_out_bound{};
                pointer tmp=map_pointer[index];
                cur=tmp+_index;
                first=tmp;
                finish=tmp+Bufsize-1;
                return *this;
            }

            inline iterator& operator-=(const size_type& n){
                size_type now=Bufsize*index+size_type(cur-first);
                size_type res=now-n;
                if(res<0) throw index_out_bound{};
                index=res/Bufsize;
                size_type _index=res%Bufsize;
                pointer tmp=map_pointer[index];
                cur=tmp+_index;
                first=tmp;
                finish=tmp+Bufsize-1;
                return *this;
            }

            inline iterator operator+(const size_type& n)const{
                iterator res{*this};
                return (res+=n);
            }

            inline iterator operator-(const size_type& n)const{
                iterator res{*this};
                return (res-=n);
            }

            inline difference_type operator-(const iterator& other)const{
                return (index!=other.index)?(index-other.index-1)*Bufsize+(cur-first)+(other.finish-other.cur)+1:(cur-other.cur);
            }

            inline iterator operator++(int){
                iterator res{*this};
                ++(*this);
                return res;
            }

            inline iterator operator--(int){
                iterator res{*this};
                ++(*this);
                return res;
            }

            inline iterator& operator++(){
                if(cur<finish) cur++;
                else {
                    first=cur=map_pointer[++index];
                    finish=map_pointer[index]+(Bufsize-1);
                }
                return *this;
            }

            inline iterator& operator--(){
                if(cur>first) --cur;
                else{
                    cur=finish=map_pointer[--index]+Bufsize-1;
                    first=map_pointer[index];
                }
                return *this;
            }

            inline bool operator==(const iterator& other)const{
                return (d==other.d&&index==other.index&&cur==other.cur);
            }

            inline bool operator!=(const iterator& other)const{
                return !(*this==other);
            }

            inline bool operator<(const iterator& other)const{
                if(d!=other.d) throw wrong_iterator{};
                if(index==other.index) return (cur-first<other.cur-other.first);
                return index<other.index;
            }

            inline bool operator<=(const iterator& other)const{
                return *this<other||*this==other;
            }

            inline bool operator>(const iterator& other)const{
                return !(*this<=other);
            }

            inline bool operator>=(const iterator& other)const{
                return !(*this<other);
            }

            inline bool operator==(const const_iterator& other)const{
                return (d==other.d&&index==other.index&&cur==other.cur);
            }

            inline bool operator!=(const const_iterator& other)const{
                return !(*this==other);
            }

            inline bool operator<(const const_iterator& other)const{
                if(d!=other.d) throw wrong_iterator{};
                if(index==other.index) return (cur-first<other.cur-other.first);
                return index<other.index;
            }

            inline bool operator<=(const const_iterator& other)const{
                return *this<other||*this==other;
            }

            inline bool operator>(const const_iterator& other)const{
                return !(*this<=other);
            }

            inline bool operator>=(const const_iterator& other)const{
                return !(*this<other);
            }

            inline pointer operator->()const{
                return cur;
            }

            inline reference_type operator*() const {
                return *cur;
            }

        };

        class const_iterator{
            friend class deque;
            friend class iterator;
            private:
            // 变量的含义同iterator
            const deque* d=nullptr; // const_iterator与iterator的区别1所在 注意 const在前 可以修改指针值，不能修改指针指向的值
            value_type** map_pointer=nullptr;
            size_type Bufsize=0;
            size_type index=0;
            pointer cur=nullptr,first=nullptr,finish=nullptr;

            inline void copy(const const_iterator& other){
                d=other.d;
                cur=other.cur;
                first=other.first;
                finish=other.finish;
                Bufsize=other.Bufsize;
                index=other.index;
                map_pointer=other.map_pointer;
            }

            inline void move(const_iterator& other){
                d=other.d;
                cur=other.cur;
                first=other.first;
                finish=other.finish;
                Bufsize=other.Bufsize;
                index=other.index;
                map_pointer=other.map_pointer;
            }

            public:
            class wrong_iterator:std::exception{
                public:
                std::string message="the two const_iterator does not refer to the same deque\n";

                inline const char* what() const throw() override{return message.c_str();}
            };

            const_iterator(const deque* _d):d{_d},Bufsize{d->Bufsize},map_pointer{d->map},
                        cur{map[index]},first{map[index]},finish {map[index]+Bufsize-1}{}

            const_iterator(const const_iterator& other){
                copy(other);
            }

            const_iterator(const_iterator&& other){
                move(other);
            }

            const_iterator(iterator& other):d{other.d},Bufsize{other.Bufsize},map_pointer{other.map_pointer},
                                                    index{other.index},first{other.first},finish{other.finish}{}

            const_iterator(){}

            ~const_iterator(){}

            inline const_iterator& operator=(const const_iterator& other){
                copy(other);
                return *this;
            }

            inline const_iterator& operator=(iterator&& other){
                copy(other);
                return *this;
            }

            inline const_iterator& operator+=(size_type n){
                size_type now=Bufsize*index+size_type(cur-first);
                size_type res=now+n;
                index=res/Bufsize;
                size_type _index=res%Bufsize;
                if(index>=d->map_size&&_index) throw index_out_bound{};
                pointer tmp=map_pointer[index];
                cur=tmp+_index;
                first=tmp;
                finish=tmp+Bufsize-1;
                return *this;
            }

            inline const_iterator& operator-=(size_type n){
                size_type now=Bufsize*index+size_type(cur-first);
                size_type res=now-n;
                if(res<0) throw index_out_bound{};
                index=res/Bufsize;
                size_type _index=res%Bufsize;
                pointer tmp=map_pointer[index];
                cur=tmp+_index;
                first=tmp;
                finish=tmp+Bufsize-1;
                return *this;
            }

            inline const_iterator operator+(const size_type& n)const{
                iterator res{*this};
                return (res+=n);
            }

            inline const_iterator operator-(const size_type& n)const{
                iterator res{*this};
                return (res-=n);
            }

            inline const_iterator& operator++(int){
                iterator res{*this};
                ++(*this);
                return res;
            }

            inline const_iterator& operator--(int){
                iterator res{*this};
                --(*this);
                return res;
            }

            inline const_iterator& operator++(){
                if(cur<finish) cur++;
                else {
                    first=cur=map_pointer[++index];
                    finish=map_pointer[index]+(Bufsize-1);
                }
                return *this;
            }

            inline const_iterator& operator--(){
                if(cur>first) --cur;
                else{
                    cur=finish=d->map[--index]+Bufsize-1;
                    first=d->map[index];
                }
                return *this;
            }

            inline bool operator==(const const_iterator& other)const{
                return (d==other.d&&index==other.index&&cur==other.cur);
            }

            inline bool operator!=(const const_iterator& other)const{
                return !(*this==other);
            }

            inline bool operator<(const const_iterator& other)const{
                if(*this==other) throw wrong_iterator{};
                if(index==other.index) return (cur-first<other.cur-other.first);
                return index<other.index;
            }

            inline bool operator<=(const const_iterator& other)const{
                return *this<other||*this==other;
            }

            inline bool operator>(const const_iterator& other)const{
                return !(*this<=other);
            }

            inline bool operator>=(const const_iterator& other)const{
                return !(*this<other);
            }

            inline bool operator==(const iterator& other)const{
                return (d==other.d&&index==other.index&&cur==other.cur);
            }

            inline bool operator!=(const iterator& other)const{
                return !(*this==other);
            }

            inline bool operator<(const iterator& other)const{
                if(d!=other.d) throw wrong_iterator{};
                if(index==other.index) return (cur-first<other.cur-other.first);
                return index<other.index;
            }

            inline bool operator<=(const iterator& other)const{
                return *this<other||*this==other;
            }

            inline bool operator>(const iterator& other)const{
                return !(*this<=other);
            }

            inline bool operator>=(const iterator& other)const{
                return !(*this<other);
            }

            inline const_pointer operator->()const{
                return cur;
            }

            inline const_reference_type operator*() const {
                return *cur;
            }
        };



        protected: //访问控制为protected，便于作为底层容器实现队列和栈！！！
        size_type Bufsize;//缓冲区的大小（以value_type为计量）
        size_type map_size=INITIAL_SIZE;// 表示中控器map里面有多少指针
        map_pointer map;//中控器map
        size_type capacity;// 表示deque总的容量
        size_t currentSize=0;//当前大小
        iterator first,finish; // first是begin，finish是end(即最后一个元素的下一个)
        allocator_type allocator=std::allocator<T>{};// 分配器 默认使用std::allocator<T>{}

        // assign的工具函数，在clear()后重新分配一些基础参数；
        inline void resize(size_type n){
            Bufsize=_deque_buf_size(BufferSize,sizeof(value_type));
            map_size=std::max(n/Bufsize+1,INITIAL_SIZE);
            capacity=Bufsize*map_size;
            map=new pointer[map_size];
            Allocate();
            Init_First_Finish(currentSize=0);
        }

        // 移动构造函数和pub-move的工具函数
        inline void move(deque& other){
            map=other.map;
            map_size=other.map_size;
            capacity=other.capacity;
            currentSize=other.currentSize;
            first=other.first;
            finish=other.finish;
            other.capacity=other.map_size=other.currentSize=0;
            other.map=nullptr;
        }

        // swap的工具函数 是否必要？
        inline void move_to(deque& other){
            other.map=map;
            other.map_size=map_size;
            other.capacity=capacity;
            other.currentSize=currentSize;
            other.first=first;
            other.finish=finish;
            map=nullptr;
            map_size=capacity=currentSize=0;
        }

        // 拷贝操作的工具函数
        inline void copy_map(const deque& other){
            allocator=other.allocator;
            currentSize=other.currentSize;
            map_size=other.map_size;
            capacity=other.capacity;
            Bufsize=other.Bufsize;
            map=new pointer[map_size];
            Allocate();
            for(size_type i=0;i<map_size;++i) copy_node(i,other);
        }

        inline void copy_node(size_type index,const deque& other){
            pointer p=map[index],q=other.map[index];
            for(size_type i=0;i<Bufsize;++i) p[i]=q[i];
        }

        inline void copy_backward(iterator from,iterator old_end,iterator new_end){
            // 将元素整体向后移动 由[from,old_end]到[new_first,new_end];
            if(from>old_end) return;
            difference_type diff_index=new_end-old_end;
            iterator new_first=from+diff_index,tmp=new_end;
            while(from<=old_end){
                *tmp=*old_end;
                --tmp;--old_end;
            }
        }

        inline void copy_to_front(iterator from,iterator old_begin,iterator new_begin){
            // 将元素整体向前移动 由[old_begin,from]到[new_begin,new_finish];
            if(old_begin>from) return;
            difference_type diff_index=old_begin-new_begin;
            iterator new_finish=from-diff_index,tmp=new_begin;
            while(from>=old_begin){
                *tmp=*old_begin;
                ++tmp;++old_begin;
            }
        }

        inline void Allocate(){
            // 分配空间的函数 给map分配空间 并处理异常
            // 使用try块解决分配失败的问题
            try{
                for(size_type i=0;i<map_size;++i) map[i]=new value_type[Bufsize];
            }
            catch(...){
                std::cout<<"ALLocate Fails\n";
                clear(); // 分配失败则全部删除
            }
        }

        inline void reallocate_map(){
            // 注意，只复制每个缓冲区的指针值即可；
            map_pointer tmp=map;
            size_type low_1=first.index,high_1=finish.index,first_row_index=first.cur-first.first,finish_row_index=finish.cur-finish.first;
            size_type index_margin=high_1-low_1;
            map=new pointer[map_size<<=1]{nullptr};
            size_type first_col_index=size_type((map_size-(currentSize-1)/Bufsize)>>1);
            size_type low_2=first_col_index;
            for(size_type i=0;i<low_2;++i) map[i]=new value_type [Bufsize];
            while(low_1<=high_1){
                map[low_2++]=tmp[low_1++]; // 注意，无需重新申请空间，只要赋值指针即可；
            }
            for(size_type i=low_2;i<map_size;++i) map[i]=new value_type [Bufsize];
            delete[] tmp;
            capacity=Bufsize*map_size;
            first.index=first_col_index;
            finish.index=first_col_index+index_margin;
            first.cur=map[first_col_index]+first_row_index;
            first.first=map[first_col_index];
            first.finish=map[first_col_index]+Bufsize-1;
            finish.cur=map[finish.index]+finish_row_index;
            finish.first=map[finish.index];
            finish.finish=map[finish.index]+Bufsize-1;
            first.map_pointer=finish.map_pointer=map;
        }

        inline void allocate_node(const_reference_type val){
            // 就是给n个数据成员赋相同的值
            pointer first_pointer=first.ptr,finish_pointer=finish.ptr;
            size_type index=first.index;
            iterator tmp=first;
            while(tmp!=finish){
                *tmp++=val;
            }
        }

        inline void Init_First_Finish (size_type num_nodes){
            // 初始化first和finish两个迭代器，尽量使之指向map的中间区域
            first=iterator{this,size_type((map_size-num_nodes/Bufsize)>>1),0};
            finish=first+size_type(num_nodes);
        }

        // 确定每个缓冲区的大小 具体计算方式为：默认最小值为INITIAL_SIZE，计算值为缓冲区大小(按字节记)/sizeof(value_type),两者取较大值
        inline size_type _deque_buf_size(size_type n,size_type sz) const{
            // return (n!=0?n:(sz>512?size_type(1):size_type(521/sz)));
            return std::max(n,sz>512?size_type(1):size_type(521/sz));
        }

        // insert 系列函数的工具函数
        inline iterator insert_aux(iterator pos,const value_type& val,size_type n){
            size_type elems_before=pos-first;
            if(elems_before<(currentSize>>1)){
                for(size_type i=0;i<n;++i) push_front(val);
                iterator tmp=first+elems_before;
                copy_to_front(first+n+elems_before-1,first+n,first);
                size_type i=0;
                while(i<n){
                    *tmp=val;
                    ++tmp;++i;
                }
                return tmp-n;
            }
            else{
                for(size_type i=0;i<n;++i) push_back(val);
                iterator move_res_begin=finish-(currentSize-n-elems_before);
                copy_backward(first+elems_before,finish-1-n,finish-1);
                iterator tmp=first+elems_before;
                while(tmp<move_res_begin){
                    *tmp=val;
                    ++tmp;
                }
                return tmp-n;
            }
        }

        // push_back 工具函数
        inline void push_back_aux(const value_type& val){
            if(finish.index==map_size-1) reallocate_map();
            *(finish.cur)=val;
            finish.first=finish.cur=map[++finish.index];
            finish.finish=map[finish.index]+Bufsize-1;
        }

        // push_front 工具函数
        inline void push_front_aux(const value_type& val){
            if(first.index==0) reallocate_map();
            first.cur=first.finish=map[--first.index]+Bufsize-1;
            first.first=map[first.index];
            *first.cur=val;
        }


        public:
        class exception : public std::exception{
            public:
            std::string message="";

            exception(const std::string& s):message{s}{}
            inline const char* what() {return message.c_str();}
        };

        class index_out_bound : public std::exception{
            public:
            std::string error="index out bound\n";

            inline const char* what(){return error.c_str();}
        };

        deque(allocator_type A=std::allocator<T>{}):Bufsize{_deque_buf_size(BufferSize,sizeof(value_type))},
                                                capacity{Bufsize*map_size},map{new pointer[map_size]},allocator{A}{
            Allocate();
            Init_First_Finish(0);
        }

        deque(size_type n,const value_type& val=value_type{},allocator_type my_allocator=std::allocator<T>{}):Bufsize{_deque_buf_size(n,size_type(sizeof(val)))},
                                                                                map_size{std::max(n/Bufsize+1,INITIAL_SIZE)},
                                                                                map{new value_type**[map_size]},capacity{Bufsize*map_size},allocator{my_allocator}{
            // assert(n>0,"the first argument must be greater than zero\n");
            Allocate();
            Init_First_Finish(n/Bufsize);
            allocate_node(val);
        }

        deque(const deque& other){
            copy_map(other);
            Init_First_Finish(other.currentSize);
        }

        deque(deque&& other){
            move(other);
        }

        // 支持使用初始化器列表(C++新特性)进行初始化

        // deque(const std::initializer_list<value_type>& lst){
        //     Bufsize=_deque_buf_size(lst.size(),sizeof(value_type));
        //     map_size=std::max(lst.size()/Bufsize+1,INITIAL_SIZE);
        //     map=new pointer[map_size];
        //     capacity=size_type(Bufsize*map_size);
        //     for(auto i:lst) push_back(i);
        // }

        deque(const std::initializer_list<value_type>& lst):
        Bufsize{_deque_buf_size(lst.size(),sizeof(value_type))}, map_size{std::max(lst.size()/Bufsize+1,INITIAL_SIZE)},
        map{new pointer[map_size]},capacity{size_type(Bufsize*map_size)}{
            Allocate();
            Init_First_Finish(0);
            for(auto i:lst) push_back(i);
        }

        deque(const std::initializer_list<value_type>& lst,allocator_type my_allocator):
                                                    Bufsize{_deque_buf_size(lst.size(),sizeof(value_type))},
                                                    map_size{std::max(lst.size()/Bufsize+1,INITIAL_SIZE)},
                                                    map{new pointer[map_size]},capacity{Bufsize*map_size},
                                                    allocator{my_allocator}{
            Allocate();
            Init_First_Finish(0);
            for(auto i:lst) push_back(i);
        }

        deque(iterator _begin,iterator _end,allocator_type my_allocator=std::allocator<T>{}):
                                                                Bufsize{_deque_buf_size(_end-_begin,sizeof(value_type))},
                                                                map_size{std::max(INITIAL_SIZE,(_end-_begin)/Bufsize+1)},
                                                                map{new pointer[map_size]},capacity{Bufsize*map_size},
                                                                allocator{my_allocator}{
            Allocate();
            Init_First_Finish(0);
            for(auto itr=_begin;itr!=_end;++itr) push_back(*itr);
        }

        ~deque(){
            clear();
        }

        // 三个重载的赋值运算符版本
        inline deque& operator=(const deque& other){
            clear();
            copy_map(other);
            return *this;
        }

        inline deque& operator=(const std::initializer_list<value_type>& lst){
            clear();
            move(deque{lst});
            return *this;
        }

        inline deque& operator=(deque&& other){
            move(other);
            return *this;
        }

        // move函数  注意其形参是&还是&& 理解右值引用与左值引用的区别与联系
        deque& move(deque&& other){
            clear();
            move(other);
            return *this;
        }

        inline void assign(size_type n,value_type val){
            clear();
            resize(n);
            for(size_type i=0;i<n;++i) push_back(val);
        }

        inline void assign(std::initializer_list<value_type>& lst){
            clear();
            resize(lst.size());
            const_pointer p=lst.begin();
            for(size_type i=0;i<lst.size();++i) push_front(p[i]);
        }

        inline void assign(iterator _begin,iterator _end){
            clear();resize(_end-_begin);
            while(_begin!=_end){
                push_back(*begin++);
            }
        }

        inline size_type size() const {
            return currentSize;
        }

        inline bool empty() const {
            return (currentSize==0);
        }

        inline size_type max_size() const{
            return capacity;
        }

        //hard to fulfill
        inline void shrink_to_fit() {}

        inline void clear(){
            for(size_type i=0;i<map_size;++i) {
                pointer del=map[i];
                if(map[i]){
                    // 需显示调用析构函数 释放内存 此时map[i]无需显示delete[]
                    for(size_type j=0;j<Bufsize;++j) del[j].~value_type();
                }
                map[i]=nullptr;
            }
            // delete[] map;
            currentSize=map_size=capacity=0;
        }

        inline iterator begin() const {
            return first;
        }

        inline iterator end() const {
            return finish;
        }

        inline const_iterator cbegin() const{
            return const_iterator{first};
        }

        inline const_iterator cend() const{
            return const_iterator{finish};
        }

        // 注意反向迭代器与正向迭代器的关系
        inline iterator rbegin()const{
            iterator res{finish};
            return (--res);
        }

        inline iterator rend()const{
            iterator res{first};
            return (--res);
        }

        inline const_iterator crbegin()const{
            return const_iterator{this->rbegin()};
        }

        inline const_iterator crend()const{
            return const_iterator{this->rend()};
        }

        inline reference_type front() const {
            if(empty()) throw index_out_bound{};
            return *first;
        }

        inline reference_type back() const {
            if(empty()) throw index_out_bound{};
            return *(finish-1);
        }

        inline reference_type operator[](size_t index) const{
            iterator tmp=first+index;
            return *tmp;
        }

        inline reference_type at(size_t index) const {
            if(index>currentSize) throw index_out_bound{};
            iterator tmp=first+index;
            return *tmp;
        }

        inline iterator insert(iterator pos,value_type val){
            if(pos==first) {
                push_front(val);
                return first;
            }
            else if(pos==finish){
                push_back(val);
                return (finish-1);
            }
            else {
                return insert_aux(pos,val,1);
            }
        }

        inline iterator insert(iterator pos,size_t n,value_type val){
            return insert_aux(pos,val,n);
        }

        inline iterator insert(iterator pos,iterator first,iterator last) {
            // 注意 插入[first,last)
            if(first==last) insert(pos,*first);
            size_type elems_before=pos-this->first,insert_elems=last-first;
            if(elems_before<currentSize>>1){
                for(size_type i=0;i<insert_elems;++i) push_front(*first);
                copy_to_front(pos-1,pos-elems_before,this->first);
                iterator tmp=this->first+elems_before;
                while(tmp<pos){
                    *tmp=*first;
                    ++first;++tmp;
                }
                return this->first+elems_before;
            }
            else{
                for(size_type i=0;i<insert_elems;++i) push_back(*first);
                copy_backward(pos,pos+(currentSize-elems_before)-1,finish-1);
                iterator tmp=this->first+elems_before;
                while(tmp<finish-(currentSize-elems_before)){
                    *tmp=*first;
                    ++first;++tmp;
                }
                return this->first+elems_before;
            }
        }

        inline iterator insert(iterator pos,std::initializer_list<value_type>& lst){
            if(lst.size()==0) return pos;
            size_type elems_before=pos-first,insert_elems=lst.size(),i=0;
            if(elems_before<currentSize>>1){
                for(size_type i=0;i<insert_elems;++i) push_front(*(lst.begin()));
                copy_to_front(pos-1,pos-elems_before,this->first);
                iterator tmp=this->first+elems_before;
                const_pointer p=lst.begin();
                while(i<insert_elems){
                    *tmp=p[i];
                    ++tmp;++i;
                }
                return this->first+elems_before;
            }
            else{
                for(size_type i=0;i<insert_elems;++i) push_back(*(lst.begin()));
                copy_backward(pos,pos+(currentSize-elems_before)-1,finish-1);
                iterator tmp=this->first+elems_before;
                const_pointer p=lst.begin();
                while(i<insert_elems){
                    *tmp=p[i];
                    ++tmp;++i;
                }
                return this->first+elems_before;
            }
        }

        // erase的使用需注意 由于erase采用的是判断需移动的元素数量来选择函数调用 因此在使用erase连续删除时，需积极利用返回值，而不能
        // 在原迭代器上进行操作，否则会产生未定义的行为(可以使用STL中的deque进行尝试)
        inline iterator erase(iterator pos){
            currentSize--;
            difference_type diff_index=pos-first;
            iterator nxt {pos};
            --nxt;
            if(diff_index<(currentSize-1>>1)){
                copy_backward(first,nxt,pos);
                ++first;
                return pos+1;
            }
            else{
                copy_to_front(finish-1,pos+1,pos);
                --finish;
                return pos;
            }
        }

        inline iterator erase(iterator _first,iterator _last){
            //注意last位置的元素不删除
            difference_type diff_index=_last-_first+1;
            currentSize-=diff_index;
            if(diff_index==currentSize) {
                clear();
                return finish; //注意此时clear对finish的处理；
            }
            size_type elems_to_move=_first-first;
            if(elems_to_move<currentSize-diff_index>>1){
                copy_backward(first,_first-1,_last-1);
                first+=diff_index;
            }
            else {
                copy_to_front(finish-1,_last,_first);
                finish-=diff_index;
            }
            return _last;
        }

        inline bool operator==(const deque& other) const{
            if(currentSize!=other.size()) return false;
            iterator tmp=first,other_tmp=other.first;
            while(tmp!=finish){
                if(*tmp!=*other_tmp) return false;
                tmp++;other_tmp++;
            }
            return true;
        }

        inline bool operator!=(const deque& other) const{
            return !(*this==other);
        }

        inline bool operator<(const deque& other) const{
            iterator this_low=first,other_low=other.first;
            while(this_low!=finish&&other_low!=other.finish){
                if(*this_low++!=*other_low++) return (*this_low<*other_low);
            }
            if(currentSize==other.currentSize) return false;
            else return (currentSize<other.currentSize);
        }

        inline bool operator<=(const deque& other) const{
            return (*this==other||*this<other);
        }

        inline bool operator>(const deque& other) const {
            return !(*this<=other);
        }

        inline bool operator>=(const deque& other) const {
            return !(*this<other);
        }

        inline void swap(deque& other){
            deque tmp;
            move_to(tmp);
            move(other);
            other.move(tmp);
        }

        inline void push_back(const value_type& val){
            currentSize++;
            if(finish.cur<finish.finish){
                *(finish.cur)=val;
                ++finish.cur;
            }
            else push_back_aux(val);
        }

        inline void pop_back(){
            currentSize--;
            --finish;
        }

        // inline void emplace(iterator pos,std::initializer_list<...>& lst){}// 注意参数args类型

        inline void push_front(const value_type& val){
            currentSize++;
            if(first.cur>first.first){
                --first.cur;
                *first.cur=val;
            }
            else push_front_aux(val);
        }

        inline void pop_front(){
            currentSize--;
            ++first;
        }

        friend inline void swap(const deque& d1,const deque& d2){
            d1.swap(d2);
        }

        // some functions created for for me to debug
        // inline bool check() const{return (first.cur>first.finish) ;}
        // inline size_type get_Buf()const{return Bufsize;}
    };

}


#endif /* MySTL_Deque */
