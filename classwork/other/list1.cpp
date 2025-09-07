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
    new_next->next = prev_ref->next;
    new_next->key = new_key;
    prev_ref->next = new_next;
}

void destroy_list(Node*& head_ref){
    if(head_ref == nullptr) return;
    while(head_ref != nullptr){
        Node* buf = head_ref;
        head_ref = head_ref->next;
        delete buf;
    }
}

void print_list(Node* head_ref){
    if(head_ref == nullptr) {
        std::cout << std::endl;
        return;
    }
    while(head_ref->next != nullptr){
        std::cout << head_ref->key << " ";
        head_ref = head_ref->next;
    }
    std::cout << head_ref->key << std::endl;
}

Node* find(Node* head_ref, int key){
    while(head_ref != nullptr && head_ref->key != key) head_ref = head_ref->next;
    return head_ref;
}


void remove_key(Node*& head_ref, int key){
    if(head_ref == nullptr) return;
    Node *buf = head_ref;
    if(head_ref->key == key){
        Node *next = head_ref->next;
        delete head_ref;
        head_ref = next;
        return;
    }
    while(buf->next != nullptr && buf->next->key != key) buf = buf->next;
    if(buf->next == nullptr) return;
    Node *next = buf->next->next;
    delete buf->next;
    buf->next = next;
}




int main() {

    
    return 0;
}