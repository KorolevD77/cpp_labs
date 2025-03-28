#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>


#define N 1000000
//#define MAX 1000000


int step(int cnt_0){
    return cnt_0 >= 1000 ? (cnt_0 >= 10000 ? (cnt_0 >= 100000 ? 100000 : 10000): 1000) : 100;
}

/*
void sort(int (&a)[N], unsigned int (&cnt)){
    for(int i = 0; i < cnt - 1; i++){
        for(int j = i + 1; j < cnt; j++){
            if(a[i] > a[j]){
                std::swap(a[i], a[j]);
            }
        }
    }
}
*/

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
        mid = l + (r - l) / 2;  
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
        for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)){             //изменяем шаг, чтобы не считать слишком долго 
            std::uniform_int_distribution <unsigned> dstr(0, max_rand[j]);
            for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);

            //sort(x, cnt);         
            //std::sort(x, x+cnt);      // раскомментить для find_O_logn
            auto begin = std::chrono::steady_clock::now();
            for(unsigned k = 100000; k != 0; --k)

            //find_O_logn(cnt, x, dstr(rng)); //                        тут исследуемая функция
            find_O_n(cnt, x, dstr(rng));

            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
            std::cout << time_span.count() << ", ";
        }
    }
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)) std::cout << cnt << ", ";       //для графика, размеры массива

    return 0;
}
