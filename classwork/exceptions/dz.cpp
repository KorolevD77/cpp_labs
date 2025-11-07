#include <iostream>

//                              №1
class error_t{
    std::string err;

public:
    error_t(): err{"abstract error"} {}
    error_t(std::string str): err{str} {}
    std::string what(){
        return err;
    }
    ~error_t(){}
};





//                              №2


struct control_block{
    int cnt;
    int weak_cnt;
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

template<typename T>
class shared_ptr{
private:
    control_block* cb;
    T* data;

public:
    shared_ptr(): data{nullptr}, cb{new control_block{0, 0}} {};
    shared_ptr(T* ptr): data{ptr}, cb{new control_block{1, 0}}{};
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


template<typename T, typename... Args>
void make_shared(Args&&... args){

}




int main(){


    return 0;
}
