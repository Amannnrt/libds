#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    int height;

    struct Node* left;
    struct Node* right;
}Node;

int max(int a,int b){
    if(a>b){
        return a;
    }
    return b;
}
int height(Node* node){
    if(node == NULL){
        return -1;
    }
    return node->height;
}

Node* create_node(int value){
    Node* new_node = malloc(sizeof(Node));
    if(new_node == NULL){
        return NULL;
    }

    new_node->data = value;
    new_node->height = 0;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

Node *right_rotate(Node *p)
{
    // Initial configuration
    Node *c = p->left;
    Node *t = c->right;

    // Changing the configuration
    c->right = p;
    p->left = t;

    // p moved down, so update it first
    p->height = max(height(p->left), height(p->right)) + 1;

    // c moved up
    c->height = max(height(c->left), height(c->right)) + 1;

    return c;
}

Node *left_rotate(Node *p)
{
    // Initial configuration
    Node *c = p->right;
    Node *a = c->left;

    // Changing the configuration
    c->left = p;
    p->right = a;

    // p moved down, so update it first
    p->height = max(height(p->left), height(p->right)) + 1;

    // c moved up
    c->height = max(height(c->left), height(c->right)) + 1;

    return c;
}


Node* rotate(Node* node){
    int balance = height(node->left) - height(node->right);
    
    if(balance>1){
        //left left case
        if(height(node->left->left)-height(node->left->right)>0){
            return right_rotate(node);
        }
        //left right case
        if(height(node->left->left)-height(node->left->right)<0){
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
    }
    if(balance<1){
        //right right case
        if(height(node->right->left)-height(node->right->right)<0){
            return left_rotate(node);
        }
        //right left case
        if(height(node->right->left)-height(node->right->right)>0){
            node->right = right_rotate(node->right);
            return left_rotate(node);    
        }
        

    }
    return node;
}

Node *insert(Node *node, int value){
    if (node == NULL) {
        return create_node(value);
    }

    if (value < node->data) {
        node->left = insert(node->left, value);
    }
    else if (value > node->data) {
        node->right = insert(node->right, value);
    }
    else {
        //ignore duplicates
        return node;
    }

    //The subtree may have changed height
    node->height = max(height(node->left), height(node->right)) + 1;

    // Check whether this node became unbalanced.
    return rotate(node);
}
void inorder(Node *root){
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    printf("%d(h=%d) ", root->data, root->height);
    inorder(root->right);
}

void display(Node *node, int depth)
{
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    printf("%d (h=%d)\n", node->data, node->height);

    display(node->left, depth + 1);
    display(node->right, depth + 1);
}


void destroy_tree(Node* root){
    if(root == NULL){
        return;
    }
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int main(void){
    Node* root = NULL;
    root = insert(root,30);
    root = insert(root,20);
    root = insert(root,10);

    printf("After inserting 30, 20, 10:\n");
    display(root, 0);

    printf("\nInorder:\n");
    inorder(root);
    printf("\n");

    destroy_tree(root);

    return 0;

}

















