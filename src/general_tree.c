#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node** children;
    size_t child_count;
}Node;

Node* create_node(int val){
    Node* new_node = malloc(sizeof(Node));

    new_node->data = val;
    new_node->child_count = 0;
    new_node->children = NULL;
    return new_node;
}

int add_child(Node* parent,Node* child){
    Node** new_children;

    new_children = realloc(parent->children,(parent->child_count+1)*sizeof(Node *));
    if(new_children == NULL){
        return 0;
    }

    parent->children =new_children;
    parent->children[parent->child_count] = child;
    parent->child_count++;
    return 1;
}


void destroy_tree(Node* root){
    if(root == NULL){
        return;
    }
    for(size_t i=0;i<root->child_count;i++){
        destroy_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

int main(){

}
