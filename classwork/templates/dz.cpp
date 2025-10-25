#include <iostream>
#include <utility>


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




//                                                          №3
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

//                                                          №4
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



int main() {
    //std::cout<< invoke(foo, 1,2,3);


    return 0;
}