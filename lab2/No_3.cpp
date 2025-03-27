#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include <math.h>

#define N 50000     // < 500000
#define N_OF_REPS 1

using std::cin, std::cout, std::endl, std::min, std::max, std::swap, std::string;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

int step(int cnt_0){
    return (cnt_0 >= 1000 ? (cnt_0 >= 10000 ? 5000: 1000) : 100);
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


unsigned int shell_sort_2n(unsigned int *a, int size) {
    unsigned int cnt = 0;
    for (unsigned int s = size; s > 0; s /= 2) 
        cnt += insert_sort_step(a, size, s);
    return cnt;
}

unsigned int shell_sort_hibbard(unsigned int *a, int size) {
    unsigned int cnt = 0;
    unsigned int i = std::log2(size+1);
    for (unsigned int s = size; s > 0; s = std::pow(2, i--)) 
        cnt += insert_sort_step(a, size, s);
    return cnt;
}

unsigned int fibs[29] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229};

unsigned int shell_sort_fib(unsigned int *a, int size) {
    unsigned int cnt = 0;
    int i = 0;
    for(; i < 29 && fibs[i] <= size; ++i);
    --i;
    for (unsigned int s = fibs[i]; i > 0; s = fibs[i--]) 
        cnt += insert_sort_step(a, size, s);
    return cnt;
}

int main(){
    std::default_random_engine rng(time(NULL));
    std::uniform_int_distribution <unsigned> dstr(0, N);
    string rez_cnts = "";
    unsigned int x[N];

    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)){        //изменяем шаг, чтобы не считать слишком долго  
        for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);        

        auto begin = std::chrono::steady_clock::now();
        unsigned cnt_tps = 0;
        for(unsigned j = N_OF_REPS; j != 0; --j){
            //cnt_tps += shell_sort_fib(x, cnt);   //                        тут исследуемая функция
            cnt_tps += insert_sort_step(x, cnt, 1);
            //forward_step(x, 0, cnt-1);
            //shell_sort(x, 0, cnt-1);
        }
        rez_cnts += std::to_string(cnt_tps/N_OF_REPS) + ", ";

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast <std::chrono::microseconds> (end - begin);

        std::cout << time_span.count()/N_OF_REPS << ", ";
    }
        
    
    std::cout << endl << rez_cnts << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)) std::cout << cnt << ", "; 

    return 0;
}