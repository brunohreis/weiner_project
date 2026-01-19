#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

typedef struct Node;

char txt[256];

boolean is_proper_prefix(char* x, char* y){
    if(strlen(x) < strlen(y)){
        for(int c=0; c<strlen(x); c++){
            if(x[c] != y[c])
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

// Suffix-tree node struct
typedef struct Node{
    Node* father;
    // table which represents the label of the edge towards this node
    int limits[2];
    // the children pointers for each of the possible symbols
    Node* child[256];
    // the a-link for each of the possible symbols
    Node* a_link[256];  
    // the a-test for each of the possible symbols
    boolean a_test[256];  
} Node;

Node* new_node(){
    Node* n = (Node*) malloc(sizeof(Node));

}

// Suffix-tree struct
typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
} Suf_tree;

Node* breaknode(Node* w1, Node* w2, Node* v1, Node* v2, char* text){
    if (!w1 || !w2 || !v1 || !v2) return NULL;

    if(w1 != w2 && v1 != v2 && w2->father == w1 && v2->father == v1){

        int len_v = v2->limits[1] - v2->limits[0];
        int len_w = w2->limits[1] - w2->limits[0];

        // check if x is prefix of y

        if (len_v >= len_w) return NULL;

        for (int i = 0; i < len_v; i++) {
            if (text[v2->limits[0] + i] != text[w2->limits[0] + i]) {
                return NULL; 
            }
        }

        // node w to be returned
        Node* w = (Node*) calloc(1, sizeof(Node));

        w->father = w1;
        w->limits[0] = w2->limits[0];
        w->limits[1] = w2->limits[0] + len_v;
        w1->child[text[w2->limits[0]]] = w;
        w->child[text[v2->limits[1]]] = w2;
        w2->father = w;
        w2->limits[0] = v2->limits[1];

        for(int i=0; i<256; i++){
            w->a_test[i] = w2->a_test[i];
            w->a_link[i] = NULL;
        }

        return w;
    }
}

// stack struct for storing the last path traversed, for the step 'Up' from 'Up-Link-Down'
// typedef struct Stack{
//     Node** el;
//     int capacity, size;
// } Stack;

// Stack* create_stack(int capacity){
//     Stack* s;
//     s->el = (Node**) malloc(capacity*sizeof(Stack));
//     s->size = 0;
//     return s;
// }

// void insert(Stack* s, Node* new_ele){
//     if(s->size < s->capacity){
//         s->el[s->size++] = new_ele;
//     } else{
//         prinft("Insert error: stack is full!");
//     }
// }

// Node* remove(Stack* s){
//     return s->el[s->size--];
// }




