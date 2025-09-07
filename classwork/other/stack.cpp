#include <iostream>

using std::cin, std::cout, std::endl, std::min, std::max, std::swap;


struct Node {
    char key = 0;
    Node* next = nullptr;
};

void push(Node*& head, char x) {
    Node* newNode = new Node;
    newNode->key = x;
    newNode->next = head;
    head = newNode;
}

char pop(Node*& head) {
    if (head == nullptr) {
        return '\0'; 
    }
    char x = head->key;
    Node* buf = head;
    head = head->next;
    delete buf;
    return x;
}

bool empty(Node* head) {
    return (head == nullptr);
}

int main() {

    return 0;
}