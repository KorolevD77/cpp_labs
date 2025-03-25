#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>

#define N 50000
#define N_OF_REPS 1

using std::cin, std::cout, std::endl, std::min, std::max, std::swap, std::string;


int step(int cnt_0){
    return (cnt_0 >= 1000 ? (cnt_0 >= 10000 ? 5000: 1000) : 100);
}

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
    string rez_cnts = "";
    unsigned int x[N];
    


    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)){        //изменяем шаг, чтобы не считать слишком долго  
        for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);        

        auto begin = std::chrono::steady_clock::now();
        unsigned cnt_tps = 0;
        for(unsigned j = N_OF_REPS; j != 0; --j){
            //cnt_tps += hairbrush_sort(x, cnt);   //                        тут исследуемая функция
            cnt_tps += bubble_step(x, cnt, 1);
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