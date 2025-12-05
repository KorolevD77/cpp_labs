#include <iostream>
#include <utility>
#include <array>
#include <vector>
/*
template<typename T>
class ScopePtr {
private:
    T* m_ptr;
public:
    ScopePtr(): m_ptr{nullptr} {}
    explicit ScopePtr(T* ptr): m_ptr{ptr} {}
    ScopePtr(const ScopePtr&) = delete;
    ScopePtr& operator=(const ScopePtr&) = delete;
    ScopePtr(ScopePtr&& other): m_ptr{std::exchange(other.m_ptr, nullptr)} {}
    ScopePtr& operator=(ScopePtr&& other) {
        std::swap(m_ptr, other.m_ptr);
        return *this;
    }
    explicit operator bool() const { return nullptr == m_ptr; }
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    T* get() const { return m_ptr; }
    T* release() {
        auto old_ptr = m_ptr;
        m_ptr = nullptr;
        return old_ptr;
    }
    void reset(T* new_ptr) {
        delete m_ptr;
        m_ptr = new_ptr;
    }
    ~ScopePtr() { delete m_ptr; }
};

template<typename T>
class ScopePtr<T[]> {
private:
    T* m_ptr;
public:
    ScopePtr() : m_ptr(nullptr) {}
    explicit ScopePtr(T* ptr) : m_ptr(ptr) {}
    ScopePtr(const ScopePtr&) = delete;
    ScopePtr& operator=(const ScopePtr&) = delete;
    ScopePtr(ScopePtr&& other) : m_ptr(std::exchange(other.m_ptr, nullptr)) {}
    ScopePtr& operator=(ScopePtr&& other) {
        if(this != &other) {
            delete[] m_ptr;
            m_ptr = std::exchange(other.m_ptr, nullptr);
        }
        return *this;
    }
    explicit operator bool() const {return m_ptr != nullptr;}
    T* get() const {return m_ptr;}
    T& operator[](std::size_t index) const {return m_ptr[index];}
    T* release() {return std::exchange(m_ptr, nullptr);}

    void reset(T* new_ptr = nullptr) {
        T* old_ptr = std::exchange(m_ptr, new_ptr);
        delete[] old_ptr;
    }

    ~ScopePtr() { 
        delete[] m_ptr; 
    }
};




//                                                          №1
template<typename T, typename U>
struct is_same{
    static const bool value = 0;
    ~is_same() = default;
};

template<typename T>
struct is_same<T, T>{
    static const bool value = 1;
};



//                                                          №2
template<typename... Args>
struct are_same{
    static const bool value = 0;
    ~are_same() = default;
};

template<typename T>
struct are_same<T>{
    static const bool value = 1;
};
template<typename T, typename U, typename... Args>
struct are_same<T, U, Args...>{
    static const bool value = is_same<T, U>::value*are_same<U, Args...>::value;
};

*/


//                                           №3
/*
template<typename T, unsigned int N>
class Array{
    //...
};


template<typename T>
unsigned int size(T arr){

}
template<typename T, unsigned int N>
unsigned int size(Array<T, N> arr){
    return N;
}
*/
/*
//                                             №4
int foo(int a, int b, int c) {
    return a * a * a + b * b + c; 
}

float bar(float r) {
    return 3.14f * r * r;
}

template<typename T, typename... Args>
auto invoke(T&& func, Args&&... args) {
    return std::forward<T>(func)(std::forward<Args>(args)...);
}




//                                          №5
template<typename T>
class Array{
    //...
};

template<typename T>
void flatten(const T& elem, std::ostream& out) {
    out << elem << " ";
}

template<typename T>
void flatten(const Array<T>& array, std::ostream& out) {
    for (const auto& elem : array)
        flatten(elem, out);
    
}
*/
/*
struct control_block{
    int cnt;
    int weak_cnt;
};


template<typename T>
class shared_ptr{
private:
    int* cnt;
    int* weak_cnt;
    T* data;

public:
    shared_ptr(): data{nullptr}, cnt{new int(0)}, weak_cnt{new int(0)} {};
    shared_ptr(T* ptr): data{ptr}, cnt{new int(1)}, weak_cnt{new int(0)}{};
    shared_ptr(const shared_ptr other): data(other.data), *cnt(++*other.cnt) {};

    ~shared_ptr(){
        --*cnt;
        if(!*cnt){
            delete data;
            if(!*weak_cnt){
                delete cnt;
                delete weak_cnt;
            }
        }
    }
};

template<typename T>
class weak_ptr{
private:
    int* weak_cnt;
    int* cnt;
    T* data;

    bool expired(){
        return !*cnt;
    }


public:

};

template<typename T, typename... Args>
void make_shared(Args&&... args){

}

//noexcept




template <typename T>
struct ListPair final {
    T const state;
    ListPair<T> *next_state;
};

template <typename T> 
ListPair<T>* copy(ListPair<T> const *src){
    try{
        if(src == nullptr) return nullptr;
        
        T *tek_src = src;
        ListPair<T>* new_head{src.state, src.next_state};
        ListPair<T>* new_tek = new_head;
        tek_src = tek_src.next_state;
        while(tek_src != src){
            new_tek.next_state = new ListPair{tek_src};
            tek_src = tek_src.next_state;
        }
        new_tek->next_state = new_head;
        return new_head;
    } catch(...){
        throw;
    }
}

*/

/*
#include <map>
#include <vector>
#include <algorithm>


std::vector<int> majorityKElements(const std::vector<int>& nums, const int k) {
    std::map<int, int> frequency;
    for (const auto& num : nums) {
        frequency[num]++;
    }
    
    std::vector<int> elements;
    for (const auto& [num, freq] : frequency) {
        elements.push_back(num);
    }
    
    std::sort(elements.begin(), elements.end(), 
         [&frequency](int a, int b) {
             return frequency[a] > frequency[b];
         });
    
    if (k > elements.size()) {
        return elements;
    }
    return std::vector<int>(elements.begin(), elements.begin() + k);
}




int main() {

    std::array<int, 6> a;   //тип сохр при преобр. в ptr
    std::array<int, 6>::iterator it = a.begin();    //*it == a[0]

    
    std::vector<int> vec(5, 0);

    //push_back, erase, emplace. list, set, map: *it[2] - key, val. unordered_map
    return 0;
}



#include<iostream>
#include<math.h>
#include<cstdint>

struct Zero { };
struct One { };

template <typename... Args>
uint64_t make_number(Args... args){
    return 0;
}

template <typename... Args>
uint64_t make_number(Zero z, Args... args){
    return make_number(args...);
}

template <typename... Args>
uint64_t make_number(One o, Args... args){
    return (uint64_t)pow(2, sizeof...(args)) + make_number(args...);
}

uint64_t make_number(One o){
    return 1;
}
uint64_t make_number(Zero o){
    return 0;
}
uint64_t make_number(){
    return 0;
}



int main() {
   std::cout 
       << make_number() << ' ' 
       << make_number(Zero()) << ' ' 
       << make_number(One()) << ' ' 
       << make_number(Zero(), One()) << ' ' 
       << make_number(One(), Zero()) << ' ' 
       << make_number(Zero(), One(), Zero()) << ' '
       << make_number(One(), Zero(), One());
    return 0;
}*/

#include <iostream>

int main(){
    std::cerr << "Не удалось открыть файл input.txt" << std::endl;
    return 0;
}