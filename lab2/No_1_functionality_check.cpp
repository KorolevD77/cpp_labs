#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#define N 25

using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

void forward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    for(int i = begin_idx; i < end_idx; ++i)
        if(arr[i+1] < arr[i])
            swap(arr[i], arr[i+1]);
}

void backward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    for(int i = begin_idx; i > end_idx; --i)
        if(arr[i-1] > arr[i])
            swap(arr[i], arr[i-1]);
}

void shaker_sort(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    int l = begin_idx, r = end_idx;
    while(l < r){
        forward_step(arr, l, r);
        r--; 
        backward_step(arr, r, l);
        l++;
    }
}



int main(){
    std::default_random_engine rng(time(NULL));
    std::uniform_int_distribution <unsigned> dstr(0, N);
    unsigned int x[N];
    

	// если отсортируется, значит, работает
	
    bool f = 1;     //верно, пока все нормально сортируется
    for(int j = 0; j < 1000; ++j){
        for(unsigned i = 0; i < N; ++i)
            x[i] = dstr(rng); 

        for(int i = 0; i < N; ++i)
            //forward_step(x, 0, N-i+1);
            backward_step(x, N, i);                 //тут исследуемая функция
        
        for(int i = 0; i < N-1; ++i)
            f *= (x[i] <= x[i+1]);
    }
    cout << f << endl;


    bool g = 1;     //верно, пока все нормально сортируется
    for(int j = 0; j < 1000; ++j){
        for(unsigned i = 0; i < N; ++i)
            x[i] = dstr(rng); 

        shaker_sort(x, 0, N-1);                 //тут исследуемая сортировка
        
        for(int i = 0; i < N-1; ++i)
            g *= (x[i] <= x[i+1]);
    }
    cout << g << endl;
    

    return 0;
}