#include <iostream>
#include <chrono>
#include <random>

#define N 1000000
#define MAX 1000000


int step(int cnt_0){
    return (cnt_0 >= 1000 ? (cnt_0 >= 10000 ? (cnt_0 >= 100000 ? 100000 : 10000): 1000) : 100);
}

int find_O_n_A(int gr, int (&arr)[N], int x) {
    int rez = 0;
    for(; rez < gr; ++rez) 
        if(arr[rez] == x){
            if(rez == 0) return rez;
        else {
            std::swap(arr[rez], arr[0]);
            return rez;
        }
    }
    return -1;
}

int find_O_n_B(int gr, int (&arr)[N], int x) {
    int rez = 0;
    for(; rez < gr; ++rez) 
        if(arr[rez] == x){
            if(rez == 0) return rez;
        else {
            std::swap(arr[rez], arr[rez-1]);
            return rez;
        }
    }
    return -1;
}

int find_O_n_C(int gr, int (&arr)[N], int x, int (&arr0)[N]) {
    int rez = 0;
    for(; rez < gr; ++rez) 
        if(arr[rez] == x){
            arr0[rez]++;
            if(rez == 0) return rez;
        else{
            if(arr0[rez] > arr0[rez-1]){
                std::swap(arr[rez], arr[rez-1]);
                std::swap(arr0[rez], arr0[rez-1]);
                return rez;
            }
        }
    }
    return -1;
}

int main(){
    std::default_random_engine rng(time(NULL));
    std::uniform_int_distribution <unsigned> dstr(0, MAX);
    int x[N];
    

    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)){        //изменяем шаг, чтобы не считать слишком долго  
        for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);
        int arr0[N] = {0};
        

        auto begin = std::chrono::steady_clock::now();
        for(unsigned j = 10000; j != 0; --j){
            unsigned zn = dstr(rng);
            if (zn > MAX*0.75) zn = MAX*0.75;       // теперь MAX*0,75 выпадает сильно чаще, чем другие числа

            find_O_n_A(cnt, x, zn); //                        тут исследуемая функция
            //find_O_n_C(cnt, x, dstr(rng), arr0);
            //find_O_n_A(cnt, x, dstr(rng));
        }

        auto end = std::chrono::steady_clock::now();
        auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
        std::cout << time_span.count() << ", ";
    }
        
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt))std::cout << cnt << ", "; 

    return 0;
}
