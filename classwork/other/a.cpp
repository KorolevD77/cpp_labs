#include <iostream>
#include <string>
#include <map>

enum TraverseType {
    INORDER = 0,
    PREORDER = 1,
    POSTORDER = 2
};

struct Node {
    int key = 0;
    Node *left = nullptr, *right = nullptr, *parent = nullptr;
    int height;
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
Node* min(Node*& tree){
    if(tree == nullptr) return tree;
    Node* branch = tree;
    while(branch->left != nullptr)
        branch = branch->left;
    return branch;
}
Node* max(Node*& tree){
    if(tree == nullptr) return tree;
    Node* branch = tree;
    while(branch->right != nullptr)
        branch = branch->right;
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
void destroy_tree(Node*& tree){
    if (tree == nullptr)
        return;
    destroy_tree(tree->left);
    destroy_tree(tree->right);
    delete tree;
    tree = nullptr;
    return;
}

int main() {
    Node* tree = nullptr;
    std::string command_s;
    enum Command { FINISH, INSERT, MIN, MAX, FIND, NEXT, DELETE, DESTROY, PRINT, UNKNOWN };
    std::map<std::string, Command> ctc{
        {"insert", INSERT}, {"min", MIN}, {"max", MAX}, {"find", FIND},
        {"next", NEXT}, {"print", PRINT}, {"delete", DELETE},
        {"destroy", DESTROY}, {"finish", FINISH}
    };
    
    while (true) {
        std::cin >> command_s;
        Command command = ctc.count(command_s) ? ctc[command_s] : UNKNOWN;
        int x;
        Node* result;
        
        switch(command) {
            case INSERT:
                std::cin >> x;
                insert_node(tree, x);
                break;
                
            case MIN:
                result = min(tree);
                if (result) std::cout << result->key << std::endl;
                break;
                
            case MAX:
                result = max(tree);
                if (result) std::cout << result->key << std::endl;
                break;
                
            case FIND:
                std::cin >> x;
                result = find(tree, x);
                if (result) std::cout << result->key << std::endl;
                break;
                
            case NEXT:
                std::cin >> x;
                result = next(tree, x);
                if (result) std::cout << result->key << std::endl;
                break;
                
            case PRINT:
                std::cin >> x;
                print(tree, static_cast<TraverseType>(x));
                break;
                
            case DELETE:
                std::cin >> x;
                delete_node(tree, x);
                break;
                
            case DESTROY:
                destroy_tree(tree);
                tree = nullptr;
                break;
                
            case FINISH:
                destroy_tree(tree);
                return 0;
                break;
        }
    }
}