#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#define N 25
#define STEP 4

using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

unsigned int bubble_step(unsigned int* a, int size, int step){
    unsigned int cnt = 0;
    for(int i = 0; i < size-step+1; i += step)
        for(int j = i; j < size; j += step)
            if(a[i] > a[j]){
                swap(a[i], a[j]);
                ++cnt;
            }
    return cnt;
}
unsigned int hairbrush_sort(unsigned int* a, int size){
    unsigned int cnt = 0;
    for(int i = size/2; i > 0; i /= 2)
        cnt += bubble_step(a, size, i);
    return cnt;
}


int main(){
    std::default_random_engine rng(time(NULL));
    std::uniform_int_distribution <unsigned> dstr(0, N);
    unsigned int x[N];
    

	// если отсортируется (с шагом), значит, работает
	
    bool f = 1;     //верно, пока все нормально сортируется с шагом
    for(int j = 0; j < 1000; ++j){
        for(unsigned i = 0; i < N; ++i)
            x[i] = dstr(rng); 

        bubble_step(x, N, STEP);                 //тут исследуемая функция
        
        for(int i = 0; i < N/STEP; ++i)
            f *= (x[i*STEP] <= x[(i+1)*STEP]);
    }
    cout << f << endl;


    	// если отсортируется, значит, работает
    bool g = 1;     //верно, пока все нормально сортируется
    for(int j = 0; j < 1000; ++j){
        for(unsigned i = 0; i < N; ++i)
            x[i] = dstr(rng); 

        hairbrush_sort(x, N);                 //тут исследуемая сортировка
        
        for(int i = 0; i < N-1; ++i)
            g *= (x[i] <= x[i+1]);
        if(!g) print(x, N);
    }
    cout << g << endl;
    

    return 0;
}