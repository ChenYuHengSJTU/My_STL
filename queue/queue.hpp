#ifndef MySTL_Queue
#define MySTL_Queue

#include"../deque/deque.hpp"

namespace MySTL {
    template<typename T,typename Sequence=MySTL::deque<T>>
    class queue{
        public:
        using value_type=typename Sequence::value_type;
        using reference_type=typename Sequence::reference_type;
        using const_reference_type=typename Sequence::const_reference_type;
        using size_type=typename Sequence::size_type;
        using container_type=Sequence;

        protected:
        container_type C;

        public:
        class queue_is_empty:std::exception{
            std::string message="the queue is empty\n";
            public:
            const char* what() const throw() {
                return message.c_str();
            }
        };


        explicit queue(const container_type& c ):C{c}{}

        explicit queue(const container_type&& c=container_type{}):C{c}{}

        // 包含分配器的构造函数
        template<typename A>
            explicit queue(const A& a):C{a}{}

        template<typename A>
            explicit queue(const container_type& c,const A& a):C{c,a}{}

        template<typename A>
            explicit queue(container_type&& c,const A& a):C{c,a}{}

        template<typename A>
            explicit queue(const queue& s,const A& a):C{s.C}{}

        template<typename A>
            explicit queue(queue&& s,const A& a):C{s.C}{}


        // 其它成员函数
        bool empty()const{
            return C.empty();
        }

        size_type size()const {
            return C.size();
        }

        reference_type front()const{
            return *C.begin();
        }

        reference_type back()const{
            if(empty()) throw queue_is_empty{};
            return *(C.end()-1);
        }

        const_reference_type top()const{
            if(empty()) throw queue_is_empty{};
            return *(C.cend()-1);
        }

        // 两种版本的push
        void push(const_reference_type val){
            C.push_back(val);
        }

        void push(value_type&& val){
            C.push_back(val);
        }

        void pop(){
            C.pop_front();
        }

        void swap(queue& other) noexcept {
            C.swap(other.C);
        }

    };
}

#endif /* MySTL_Queue */
