#include "weiner.h"

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

void free_node(Node* n) {
    if (!n) return;
    
    // Recursively free all children first
    for (int i = 0; i < 256; i++) {
        if (n->child[i]) {
            free_node(n->child[i]);
        }
    }
    
    // Free the node itself
    free(n);
}

// Suffix-tree struct
typedef struct Suf_tree{
    Node* root;
    Node* cur_leaf;
    char* text;
    int text_len;
} Suf_tree;

Suf_tree* new_suf_tree(char* text){
    Suf_tree* st = (Suf_tree*) calloc(1, sizeof(Suf_tree));
    st->text = text;
    st->text_len = strlen(text);
    return st;
}

void free_suf_tree(Suf_tree* st) {
    if (!st) return;

    // Free all nodes starting from root
    free_node(st->root);

    // Free the tree structure container
    free(st);
}

Node* breaknode(Node* w1, Node* w2, Node* v1, Node* v2, char* text){
    if (!w1 || !w2 || !v1 || !v2) return NULL;

    if(w1 != w2 && v1 != v2){

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
        w1->child[(unsigned char)text[w2->limits[0]]] = w;
        w->child[(unsigned char)text[w2->limits[0] + len_v]] = w2;
        w2->father = w;
        w2->limits[0] += len_v;

        for(int i=0; i<256; i++){
            w->a_test[i] = w2->a_test[i];
            w->a_link[i] = NULL;
        }

        return w;
    }
}

//newSymbol(a): called when symbol 'a' does not occur in p_{i+1} the new suffix is attached directly to the root
void new_symbol(Suf_tree* st, int i){
    if (st->cur_leaf == NULL) return;
    unsigned char a = (unsigned char) st->text[i];
    Node* root = st->root;
    // safety check (should not happen in new_symbol case)
    if (root->child[a] != NULL){
        st->cur_leaf = root->child[a];
        return;
    }

    // create new leaf and connect it to root
    Node* leaf = new_node();
    root->child[a] = leaf;
    leaf->father = root;
    leaf->limits[0] = i;
    leaf->limits[1] = st->text_len;

    for(Node* n = st->cur_leaf; n != NULL && n->a_test[a] == FALSE; n = n->father)
        n->a_test[a] = TRUE;
        
    if(st->cur_leaf->a_link[a] == NULL) st->cur_leaf->a_link[a] = leaf;
    st->cur_leaf = leaf;
}

boolean occurs(char c, const char* text){
    int len = strlen(text);
    if(text != NULL && len > 0){
        for(int i=0; i<len; i++){
            if(text[i] == c) return TRUE;
        }
    }
    return FALSE;
}

Node* firsttest(Suf_tree* st, unsigned char a){
    if(st != NULL){
        Node* i;
        for(i = st->cur_leaf; i != NULL && i->a_test != NULL && !i->a_test[a]; i = i->father);
        return i;
    } 
    return NULL;
}

Node* firstlink(Suf_tree* st, unsigned char a){
    if(st != NULL){
        Node* i;
        for(i = st->cur_leaf; i != NULL && i->a_link != NULL && i->a_link[a] == NULL; i = i->father);
        return i;
    } 
    return NULL;
}

void weiner_construction(Suf_tree* st){
    // right-to-left suffix construction
    // compute the tree for the last letter i
    st->root = new_node();
    st->cur_leaf = new_node();
    int len = st->text_len; 
    int i = len - 1;
    st->cur_leaf->limits[0] = i;
    st->cur_leaf->limits[1] = len;
    st->cur_leaf->father = st->root;
    st->root->child[(unsigned char) st->text[i]] = st->cur_leaf;
    st->root->a_test[(unsigned char) st->text[i]] = TRUE;

    // for each p_i from the end to the beggining
    for(i--; i>=0; i--){
        // searches for an occurrence of a in p(i+1), starting from the current leaf and going up
        unsigned char a = (unsigned char)st->text[i];
        Node* ft = firsttest(st, a);
        // if a occurs 
        if(ft != NULL){
            Node* fl = firstlink(st, a);
            if (fl == NULL) {
                new_symbol(st, i);
                continue;
            }
            Node* w1 = fl->a_link[a];
            Node* head;
            if(fl == ft){
                head = w1;
            } else{
                // unsigned char transition_char = (unsigned char) st->text[ft->limits[0]];
                // Node* w2 = w1->child[transition_char];
                // head = breaknode(w1, w2, fl, ft, st->text);
                Node* child_on_path = ft;
                while (child_on_path->father != fl && child_on_path->father != NULL) {
                    child_on_path = child_on_path->father;
                }
                
                unsigned char transition_char = (unsigned char) st->text[child_on_path->limits[0]];
                
                Node* w2 = w1->child[transition_char];
                
                if (w2 != NULL) {
                    head = breaknode(w1, w2, fl, child_on_path, st->text);
                } else {
                    head = NULL;
                }
            }
            if (head != NULL) {
                ft->a_link[a] = head;
                
                if (st->cur_leaf && st->cur_leaf->a_link[a] == NULL)
                    st->cur_leaf->a_link[a] = head;

                for(Node* n = st->cur_leaf; n != NULL && n->a_test != NULL && !n->a_test[a]; n = n->father) 
                    n->a_test[a] = TRUE;

                st->cur_leaf = head;
            } else {
                new_symbol(st, i);
            }
        } else{
            new_symbol(st, i);
        }
    }
}

boolean search(Suf_tree* st, char* pattern) {
    if (!st || !pattern) return FALSE;

    Node* curr = st->root;
    int p_len = strlen(pattern);
    int i = 0;

    while (i < p_len) {
        unsigned char c = (unsigned char)pattern[i];
        
        // Check if edge exists
        if (!curr->child[c]) return FALSE;

        Node* next = curr->child[c];
        int start = next->limits[0];
        int edge_len = next->limits[1] - start;

        // Compare edge label with pattern
        for (int j = 0; j < edge_len; j++) {
            if (i + j >= p_len) return TRUE; // Pattern fully matched within edge
            if (st->text[start + j] != pattern[i + j]) return FALSE;
        }

        curr = next;
        i += edge_len;
    }

    return TRUE;
}