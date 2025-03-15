#include <iostream>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}



void forward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    for(int i = begin_idx; i < end_idx; ++i)
        if(arr[i+1] < arr[i])
            swap(arr[i], arr[i+1]);
}

void backward_step(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    for(int i = begin_idx; i > end_idx; --i)
        if(arr[i-1] > arr[i])
            swap(arr[i], arr[i-1]);
}

void shaker_sort(unsigned arr[], unsigned const begin_idx, unsigned const end_idx){
    int l = begin_idx, r = end_idx;
    while(l < r){
        forward_step(arr, l, r);
        r--; 
        backward_step(arr, r, l);
        l++;
    }
}



int main(){
    unsigned int a[15] = {5,8,3,4,1,7,10,89,11,3,1,8,6,0,7};
    shaker_sort(a, 0, 14);

    print(a, 15);

    return 0;
}