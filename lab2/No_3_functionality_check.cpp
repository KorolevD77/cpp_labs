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

unsigned int insert_sort_step(unsigned int *a, int size, int s){
    unsigned int cnt = 0;
    for (int i = s; i < size; ++i) 
        for (int j = i - s; j >= 0 && a[j] > a[j + s]; j -= s){
            swap(a[j], a[j+s]);
            ++cnt;
        }
    return cnt;
}
unsigned int shell_sort(unsigned int *a, int size) {
    unsigned int cnt = 0;
    for (int s = size / 2; s > 0; s /= 2) 
        cnt += insert_sort_step(a, size, s);
    return cnt;
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
        insert_sort_step(x, N, STEP);                 //тут исследуемая функция
        
        for(int j = 0; j < STEP; ++j)
            for(int i = j; i < N-STEP; i += STEP)
                f *= (x[i] <= x[i+STEP]);
        if(!f) print(x, N);
    }
    cout << f << endl;


    bool g = 1;     //верно, пока все нормально сортируется
    for(int j = 0; j < 1000; ++j){
        for(unsigned i = 0; i < N; ++i)
            x[i] = dstr(rng); 

        shell_sort(x, N);                 //тут исследуемая сортировка
        
        for(int i = 0; i < N-1; ++i)
            g *= (x[i] <= x[i+1]);
    }
    cout << g << endl;
    

    return 0;
}