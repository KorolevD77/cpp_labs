#include <iostream>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;

struct Node{
    int key = 0;
    Node* prev = nullptr;
    Node* next = nullptr;
};

struct List{
    Node* HEAD = nullptr;
    Node* TAIL = nullptr;
    Node* NIL = nullptr;
    size_t size = 0;
};

List* create_empty_list(){
    List* list = new List;
    list->NIL = new Node;
    //list->NIL->next = list->NIL->prev = list->NIL;
    list->HEAD = list->TAIL = list->NIL;
    //list->HEAD->prev = list->NIL;
    //list->TAIL->next = list->NIL;
    return list;
}

void print_list(const List* list){
    if(list == nullptr) return;
    Node* i = list->HEAD;
    for(; i->next != nullptr && i->next != list->NIL; i = i->next)
        std::cout << i->key << " ";
    std::cout << i->key << std::endl;
}


List* push_front(List* list, int new_key){
    if(list == nullptr) list = create_empty_list();
    Node* n_h_next = new Node;
    n_h_next->key = new_key;
    n_h_next->next = list->HEAD->next;
    n_h_next->prev = list->HEAD;
    list->HEAD->next = n_h_next;
    std::swap(list->HEAD->key, list->HEAD->next->key);
    return list;
}

List* insert_after(List* list_ptr, Node* prev_ptr, int new_key){
    if(prev_ptr == nullptr || list_ptr == nullptr) return list_ptr;
    Node* next = new Node;
    next->key = new_key;
    next->next = prev_ptr->next;
    next->prev = prev_ptr;
    prev_ptr->next = next;
    return list_ptr;
}

List* push_back(List* list_ptr, int new_key){
    if(list_ptr == nullptr){
        list_ptr = create_empty_list();
    }
    Node* a = new Node;
    a -> key = new_key;
    if(list_ptr -> size == 0){
        a -> next = list_ptr ->  NIL;
        a -> prev = list_ptr -> NIL;
        list_ptr -> HEAD = a;
        list_ptr -> TAIL = a;
    }
    else{
        a -> next = list_ptr -> TAIL -> next;
        a -> prev = list_ptr -> TAIL; 
        list_ptr -> TAIL -> next = a;
        list_ptr -> TAIL = a;
    }
    list_ptr -> size++;
    return list_ptr;
}


void clear_list(List* list_ptr) {
    if (list_ptr == nullptr) return;
    if (list_ptr->NIL == nullptr) return;
    if (list_ptr->HEAD == nullptr) {
        list_ptr->TAIL = nullptr;
        list_ptr->size = 0;
        return;
    }
    Node* current = list_ptr->HEAD;
    Node* next_node;
    while (current != list_ptr->NIL) {
        next_node = current->next;
        delete current;
        current = next_node;
    }
    list_ptr->HEAD = list_ptr->NIL;
    list_ptr->TAIL = list_ptr->NIL;
    list_ptr->NIL->next = list_ptr->NIL;
    list_ptr->NIL->prev = list_ptr->NIL;
    list_ptr->size = 0;
}

int main() {
    List* list = create_empty_list();
    list = push_back(list, 0);
    list = push_back(list, 1);
    clear_list(list);
    if ((list->HEAD == list->TAIL) and (list->NIL != nullptr)) {
      cout << "OK" << endl;
    }
    delete list->NIL;
    delete list;
    return 0;
  }