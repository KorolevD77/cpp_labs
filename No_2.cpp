#include <iostream>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

void bubble_step(unsigned int* a, int size, int step){
    for(int i = 0; i < size-step-1; i += step)
        for(int j = i; j < size; j += step)
            if(a[i] > a[j]) swap(a[i], a[j]);
}
void hairbrush_sort(unsigned int* a, int size){
    for(int i = size/2; i > 0; i /= 2)
        bubble_step(a, size, i);
}




int main(){
    unsigned int a[15] = {5,8,3,4,1,7,10,89,11,3,1,8,6,0,7};
    //hairbrush_sort(a, 15);
    //bubble_step(a, 15, 15/4);

    print(a, 15);

    return 0;
}