#include <iostream>
#include <chrono>
#include <random>

#define N 1000000
#define MAX 1000000
//#define SEED 1001


void ftn(int arr[], int gr, int a) {
    int left = 0;     
    int right = gr - 1;

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

int main(){
    
    std::default_random_engine rng(time(NULL));
    int max_rand[5] = {10000, 100000, 500000, 1000000, 1500000};
    int x[N];

    for(int j = 0; j < 5; ++j){
        std::cout << std::endl <<  "max: " << max_rand[j] << std::endl;
        for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100){    //изменяем шаг, чтобы не считать слишком долго  
            std::uniform_int_distribution <unsigned> dstr(0, max_rand[j]);
            for(unsigned i = 0; i < cnt; ++i) x[i] = dstr(rng);

            auto begin = std::chrono::steady_clock::now();
            for(unsigned j = 10000; j != 0; --j)

            ftn(x, cnt, dstr(rng)); //                        тут исследуемая функция

            auto end = std::chrono::steady_clock::now();
            auto time_span = std::chrono::duration_cast <std::chrono::milliseconds> (end - begin);
            std::cout << time_span.count() << ", ";
        }
        
    }
    
    std::cout << std::endl << std::endl;
    for(unsigned cnt = 100; cnt <= N; cnt +=  cnt >= 1000 ? (cnt >= 10000 ? (cnt >= 100000 ? 100000 : 10000): 1000) : 100) std::cout << cnt << ", ";







    return 0;
}

