#include <iostream>
#include <algorithm>
using std::cin, std::cout, std::endl, std::min, std::max, std::swap;

struct Node{
    int key = 0;
    Node *left = nullptr, *right = nullptr, *parent = nullptr;
};

void insert_node(Node*& tree, int new_key){
    if(tree == nullptr){
        tree = new Node;
        tree->key = new_key;
        return;
    }
    Node* branch = tree;
    while(branch != nullptr){
        if(new_key > branch->key) {
            if(branch->right == nullptr){
                branch->right = new Node;
                branch->right->key = new_key;
                branch->right->parent = branch;
                return;
            }
            else branch = branch->right;
        }
        else {
            if(branch->left == nullptr){
                branch->left = new Node;
                branch->left->key = new_key;
                branch->left->parent = branch;
                return;
            }
            else branch = branch->left;
        }
    }
}

Node* max(Node*& tree){
    if(tree == nullptr) return tree;
    Node* branch = tree;
    while(branch->right != nullptr)
        branch = branch->right;
    return branch;
}

Node* min(Node*& tree){
    if(tree == nullptr) return tree;
    Node* branch = tree;
    while(branch->left != nullptr)
        branch = branch->left;
    return branch;
}

Node* find(Node*& tree, int key){
    if(tree == nullptr) return tree;
    Node* branch = tree;
    while(branch != nullptr && branch->key != key){
        if(branch->key > key) branch = branch->left;
        else branch = branch->right;
    }
    return branch;
}

Node* next(Node*& tree, int key) {
    if(tree == nullptr)
        return tree;
    
    Node* el_key = find(tree, key);
    if(el_key == nullptr)
        return nullptr;

    if (el_key->right != nullptr)
        return min(el_key->right);

    Node* dot = nullptr;
    Node* par = tree;
    while (par != el_key) {
        if (el_key->key < par->key) {
            dot = par;
            par = par->left;
        } 
        else par = par->right;
    }
    
    return dot;
}

enum TraverseType {INORDER, PREORDER, POSTORDER};

void print_in(Node* node) {
    if (node == nullptr) return;
    print_in(node->left);
    std::cout << node->key << " ";
    print_in(node->right);
}

void print_pre(Node* node) {
    if (node == nullptr) return;
    std::cout << node->key << " ";
    print_pre(node->left);
    print_pre(node->right);
}

void print_post(Node* node) {
    if (node == nullptr) return;
    print_post(node->left);
    print_post(node->right);
    std::cout << node->key << " ";
}
void print(Node*& tree, const TraverseType type) {
    if (tree == nullptr) {
        std::cout << std::endl;
        return;
    }
    switch (type) {
        case INORDER:
            print_in(tree);
            break;
        case PREORDER:
            print_pre(tree);
            break;
        case POSTORDER:
            print_post(tree);
            break;
    }
    std::cout << std::endl;
}


void transplant(Node*& tree, Node* u, Node* v) {
    if (u->parent == nullptr) {
        tree = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}


void delete_node(Node*& tree, int key) {
    Node* to_del = find(tree, key);

    if (to_del == nullptr) 
        return; 
    
    if (to_del->left == nullptr && to_del->right == nullptr) {
        if (to_del->parent == nullptr)
            tree = nullptr;
        else {
            if (to_del->parent->left == to_del)
                to_del->parent->left = nullptr;
            else to_del->parent->right = nullptr;
        }
        delete to_del;
    }

    else if (to_del->left == nullptr || to_del->right == nullptr) {
        Node* child = (to_del->left != nullptr) ? to_del->left : to_del->right;

        if (to_del->parent == nullptr) {
            tree = child;
            if (tree != nullptr)
                tree->parent = nullptr;
            } 
            else {
            if (to_del->parent->left == to_del)
                to_del->parent->left = child;
            else to_del->parent->right = child;
             if(child != nullptr)
                child->parent = to_del->parent;
        }
        delete to_del;
    }

    else {
        Node* scr = min(to_del->right);
        to_del->key = scr->key;

        if (scr->parent->left == scr)
            scr->parent->left = scr->right;
        else scr->parent->right = scr->right;

        if(scr->right != nullptr)
            scr->right->parent = scr->parent;

        delete scr;
    }
}




int main() {
    Node* tree = nullptr;
    insert_node(tree, 4);
    insert_node(tree, 2);
    insert_node(tree, 6);
    delete_node(tree, 3);
    delete_node(tree, 2);
    delete_node(tree, 6);
    if (!(tree->left) and !(tree->right)) {
      cout << "OK" << endl;
    }
    delete_node(tree, 4);
    return 0;
  }
