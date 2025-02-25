#include <iostream>
#include <chrono>
#include <random>

#define N 1000000
#define MAX 1000000
//#define SEED 1001


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
    int max_rand[5] = {10000, 100000, 500000, 1000000, 1500000};
    int x[N];
    int arr0[N] = {0};

    for(int j = 0; j < 5; ++j){
        std::cout << std::endl <<  "max: " << max_rand[j] << std::endl;
        for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100){
            std::uniform_int_distribution <unsigned> dstr(0, max_rand[j]);
            for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);

            auto begin = std::chrono::steady_clock::now();
            for(unsigned j = 100000; j != 0; --j)
 
            //find_O_n_A(cnt, x, dstr(rng)); //                        тут исследуемая функция
            find_O_n_A(cnt, x, dstr(rng));

            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
            std::cout << time_span.count() << ", ";
        }
        
    }
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100) std::cout << cnt << ", ";

    //for(unsigned i = 0; i < cnt; ++i) std::cout << x[i] << " ";






    return 0;
}
