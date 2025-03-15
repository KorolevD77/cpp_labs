#include <iostream>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


void print(unsigned int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

/*
void vstavka(int* a, int x, int size){
    int i = x+1;
    while(i < size && a[i] < a[x]){
        std::swap(a[i], a[x]);   
        x++;
        i++;
    }
}

void vstavka_sort(int* a, int size){
    for(int i = 0; i < size-1; ++i)
        vstavka(a, size-i-2, size);
}
*/
/*
void shell_sort(int *array, int size) {
    for (int s = size / 2; s > 0; s /= 2) {

        for (int i = s; i < size; ++i) {
            for (int j = i - s; j >= 0 && array[j] > array[j + s]; j -= s) {
                int temp = array[j];
                array[j] = array[j + s];
                array[j + s] = temp;
            }
        }

    }
}
*/

void vstavka(unsigned int* a, int x, int step, int size){
    int i = x+1;
    while(i < size && a[i] < a[x]){
        std::swap(a[i], a[x]);
        x += step;
        i += step;
    }
}

void vstavka_sort_step(unsigned int* a, int step, int size){
    for(int i = 0; i < size-1; ++i)
        vstavka(a, size-i-2, step, size);
}

void shell_sort(unsigned int* a, int size){
    for(int i = size/2; i > 0; i /= 2){
        vstavka_sort_step(a, i, size);
        print(a, 15);
    }
}

int main(){
    unsigned int a[15] = {5,8,3,4,1,7,10,89,11,3,1,8,6,0,7};
    
    //shell_sort(a, 15);

    print(a, 15);

    return 0;
}