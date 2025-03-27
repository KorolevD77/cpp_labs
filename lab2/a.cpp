#include <iostream>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;

struct Node{
    int key = 0;
    Node* next = nullptr;
};

void push_front(Node*& head_ref, int new_key){
    Node* new_h = new Node;
    new_h->key = new_key;
    new_h->next = head_ref;
    head_ref = new_h;
}

void push_back(Node*& head_ref, int new_key){
    Node* new_end = new Node;
    new_end->key = new_key;
    if (head_ref == nullptr) {
        head_ref = new_end;
        return;
    }
    Node* end = head_ref;
    while(end->next != nullptr) end = end->next;
    
    end->next = new_end;
}

void insert_after(Node*& prev_ref, int new_key){
    Node* new_next = new Node;
    

}


int main() {
    
  }