#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

//#define N 100000
#define N 1000000
//#define MAX 1000000


int step(int cnt_0){
    return (cnt_0 >= 1000 ? (cnt_0 >= 10000 ? (cnt_0 >= 100000 ? 100000 : 10000): 1000) : 100);
}

void find_2_O_n(int arr[], int gr, int a, int& left, int& right) {
    left = 0;     
    right = gr - 1;

    while (left < right) {
        int sum = arr[left] + arr[right];
        if (sum == a) {
            //std::cout << left << " и " << right << std::endl;
            return;
        }
        if (sum < a) left++;
        else right--;
    }
}


void sort(int (&a)[N], unsigned int (&cnt)){
    for(int i = 0; i < cnt - 1; i++){
        for(int j = i + 1; j < cnt; j++){
            if(a[i] > a[j]){
                std::swap(a[i], a[j]);
            }
        }
    }
}

int main(){
    
    std::default_random_engine rng(time(NULL));
    int max_rand[5] = {10000, 100000, 500000, 1000000, 1500000};
    int x[N];

    for(int j = 0; j < 5; ++j){
        std::cout << std::endl <<  "max: " << max_rand[j] << std::endl;
        for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)){    //изменяем шаг, чтобы не считать слишком долго  
            std::uniform_int_distribution <unsigned> dstr(0, max_rand[j]);
            for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);

            std::sort(x, x+cnt);
            auto begin = std::chrono::steady_clock::now();
            for(unsigned j = 10000; j != 0; --j){
                int l, r;
                find_2_O_n(x, cnt, dstr(rng), l, r); //                        тут исследуемая функция
            }

            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
            std::cout << time_span.count() << ", ";
        }
        
    }
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt += step(cnt)) std::cout << cnt << ", ";


    return 0;
}

