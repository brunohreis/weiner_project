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
    Node* n = (Node*) calloc(1, sizeof(Node));
    return n;
}

// Suffix-tree struct
typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
    char* text;
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

//newSymbol(a): called when symbol 'a' does not occur in p_{i+1} the new suffix is attached directly to the root
void newSymbol(Suf_tree* st, int i){
    unsigned char a = st->text[i];
    Node* root = st->root;
    // safety check (should not happen in newSymbol case)
    if (root->child[a] != NULL){
        st->cur_leaf = root->child[a];
        return;
    }

    // create new leaf and connect it to root
    Node* leaf = create_node();
    root->child[a] = leaf;
    leaf->father = root;
    leaf->limits[0] = i;
    leaf->limits[1] = i+1;

    for(Node* n = st->cur_leaf; i != NULL; n = n->father)
        n->a_test[a] = TRUE;
        
    st->cur_leaf->a_link[a] = leaf;
    st->cur_leaf = leaf;
}



