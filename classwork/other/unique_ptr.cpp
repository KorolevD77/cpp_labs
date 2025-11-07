#include <iostream>

template<typename T>
struct remove_ref{
   using type = T;
};
template<typename T>
struct remove_ref<T&>{
   using type = T;
}; 
template<typename T>
struct remove_ref<T&&>{
   using type = T;
}; 


template<typename T>
class ScopePtr{
private:
    T *m_ptr;
public:
    ScopePtr(): m_ptr{nullptr}{}
    ScopePtr(T *ptr): m_ptr{ptr}{}
    ScopePtr(const ScopePtr& other) = delete;
    ScopePtr(ScopePtr &&other){
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }

    ScopePtr& operator(ScopePtr&& other){
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }
    ScopePtr& operator=(const ScopePtr& other) = delete;
    bool operator bool() const {return nullptr == m_ptr;}
    T& operator*() {return *m_ptr;}
    T& operator*() const {return *m_ptr;}
    T* operator->() const {return m_ptr}

    void reset(T* ptr){
        delete m_ptr;
        m_ptr = ptr;
    }
    T* release(){
        T* tmp = m_ptr;
        m_ptr = nullptr;
        return tmp;
    }
    T* get() const {
        return m_ptr;
    }
    remove_ref<T>&& move(T&& arg){
        return static_cast<remove_ref<T>&&>(arg);
    }

    ~ScopePtr(){
        delete m_ptr;
    }
};


int main() {
    
    return 0;
}






/*







template<typename T> 
T max(T a, T b){
    return (a < b ? b : a);
}

template<typename T, typename U> 
T max(T a, U b){
    return (a < (T)b ? (T)b : a);
}

template<typename T>
class Node{
    Node *next, *prev;
    T data;
};

using Node_f = Node<float>;

template<typename T = int, typename U = int>
struct Pair{
    T first;
    U second;
};


template<typename T, size_t N>
class Array{
    static_assert(N > 0 && "shit happens"); //во время компиляции
    //assert(...) - во время работы программы
};

template<size_t N>
class Array<bool, N>{
    unsigned long long *data;
};

template<typename Func>
int m_t(Func func){
    func();
}






*/