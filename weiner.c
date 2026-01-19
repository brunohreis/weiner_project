#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

typedef struct Node;

// Suffix-tree node struct
typedef struct Node{
    // matrix which represents the label of the edge, in which the line is the next symbol, the 1st column is the beggining and the 2nd is the ending index 
    int limits[256][2];
    // the children pointers for each of the possible symbols
    Node** child[256];
    // the a-link for each of the possible symbols
    Node** a_link[256];  
    // the a-test for each of the possible symbols
    boolean a_test[256];  
} Node;

// Suffix-tree struct
typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
} Suf_tree;

Node* breaknode(Node* w1, Node* w2, Node* v1, Node* v2){
    if(w1 != w2){
        
    }
}

// stack struct for storing the last path traversed, for the step 'Up' from 'Up-Link-Down'
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




