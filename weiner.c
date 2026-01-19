#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

typedef struct Node;

typedef struct Node{
    int i, j;
    Node* left;
    Node* right;
    boolean a_test[256];
    Node* a_link[256];    
} Node;

typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
} Suf_tree;

typedef struct Stack{
    Node** el;
    int capacity, size;
} Stack;

Stack* create_stack(int capacity){
    Stack* s;
    s->el = (Node**) malloc(capacity*sizeof(Stack));
    s->size = 0;
    return s;
}

void insert(Stack* s, Node* new_ele){
    if(s->size < s->capacity){
        s->el[s->size++] = new_ele;
    } else{
        prinft("Insert error: stack is full!");
    }
}

Node* remove(Stack* s){
    return s->el[s->size--];
}


