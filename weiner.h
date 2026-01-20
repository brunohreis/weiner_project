#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef char boolean;

#define TRUE 1
#define FALSE 0

typedef struct Node Node;
typedef struct Suf_tree Suf_tree;
Suf_tree* new_suf_tree(char* text);
void weiner_construction(Suf_tree* st);
void free_suf_tree(Suf_tree* st);
void free_node(Node* n);
boolean search(Suf_tree* st, char* pattern);