#include <iostream>
#include <algorithm>

/*  пузырьком, 
    выбором (ищем макс и в конец), 
    вставками (несорт|сорт ищем место, куда вставить - брать size-i-2 и пузырем)
    bucket sort: известны мин, макс ()
    слиянием: очев
*/

void print(int* a, int size){
    for(int i = 0; i < size; ++i) std::cout << a[i] << " ";
    std::cout << std::endl;
}

void bubble_sort(int* a, int size){
    for(int i = 0; i < size-1; ++i)
        for(int j = i; j < size; ++j)
            if(a[i] > a[j]) std::swap(a[i], a[j]);
}

int max_ind(int* a, int size){
    int max_i = 0;
    for(int i = 0; i < size; ++i)
        if(a[i] > a[max_i]) max_i = i;
    return max_i;
}

void vibor_sort(int* a, int size){
    for(int i = 0; i < size; ++i)
        std::swap(a[max_ind(a, size-i)], a[size-i-1]);
}

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

void merge(int* a, int* b, int* rez, int n1, int n2){
    int i = 0, j = 0;
    while(i < n1 && j < n2){
        if(a[i] <= b[j]){
            rez[i+j] = a[i];
            ++i;
        }
        if(a[i] >= b[j]){
            rez[i+j] = b[j];
            ++j;
        }
    }
    if(i == n1)
        for(; j < n2; ++j) rez[n1+j] = b[j];
    if(j == n2)
        for(; i < n1; ++i) rez[n2+i] = a[i];
}

void merge_sort(int* a, int size){
    if(size <= 1) return;
    int a1[size/2], a2[size/2 + size%2];
    for(int i = 0; i < size/2; ++i) a1[i] = a[i];
    for(int i = 0; i < size/2 + size%2; ++i) a2[i] = a[i+size/2];    
    merge_sort(a1, size/2);
    merge_sort(a2, size/2 + size%2);
    merge(a1, a2, a, size/2, size/2 + size%2);
}



int main(){
    int a[15] = {5,8,3,4,1,7,10,89,11,3,1,8,6,0,7};

    //bubble_sort(a, 15);
    //vibor_sort(a, 15);
    //vstavka_sort(a, 15); 
    //merge_sort(a, 15);

    print(a, 15);

    return 0;
}