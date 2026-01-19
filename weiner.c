#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

typedef struct Node;

typedef struct Node{
    int limits[256][2];
    Node** child[256];
    Node** a_link[256];  
    boolean a_test[256];  
} Node;

typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
} Suf_tree;

Node* breaknode(Node* w1, Node* w2, Node* v1, Node* v2){
    if(w1 != w2){
        
    }
}

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




