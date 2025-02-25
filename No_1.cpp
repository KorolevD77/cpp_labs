#include <iostream>
#include <chrono>
#include <random>

#define N 1000000
#define MAX 1000000


int find_O_n(int gr, int (&arr)[N], int x) {
    int rez = 0;
    for(; rez < gr; ++rez) 
        if(arr[rez] == x) return rez;
    return -1;
}

int find_O_logn(int gr, int (&arr)[N], int x) {
    bool flag = false;
    int l = 0;  
    int r = gr-1; 
    int mid;
    while ((l <= r) && (flag != true) && r <= gr && l >= 0) {
        mid = (l + r) / 2;  
        if (arr[mid] == x) flag = true;  
        else if (arr[mid] > x) r = mid - 1; 
        else l = mid + 1;
    }
    if(flag) return mid;
    return -1;
}

int main(){
    int x[N];
    std::default_random_engine rng(time(NULL));
    int max_rand[5] = {10000, 100000, 500000, 1000000, 1500000};

    for(int j = 0; j < 5; ++j){
        std::cout << std::endl <<  "max: " << max_rand[j] << std::endl;
        for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100){    //изменяем шаг, чтобы не считать слишком долго  
            std::uniform_int_distribution <unsigned> dstr(0, max_rand[j]);
            for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);

            auto begin = std::chrono::steady_clock::now();
            for(unsigned j = 100000; j != 0; --j)

            //find_O_logn(cnt, x, dstr(rng)); //                        тут исследуемая функция
            find_O_n(cnt, x, dstr(rng));

            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
            std::cout << time_span.count() << ", ";
        }
        
    }
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100) std::cout << cnt << ", ";







    return 0;
}
