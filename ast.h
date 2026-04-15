#ifndef _AST_H
#define _AST_H

typedef struct node {
    char *type;
    char *value;
    
    char *anotated_type; 
    int line;           
    int col;           
    
    struct node *child;
    struct node *next;
} node;

node* create_node(char* type, char* value, int line, int col); 
node* append_sibling(node* list, node* new_node);
void add_child(node* parent, node* new_child);
void print_tree(node* n, int level);

#endif