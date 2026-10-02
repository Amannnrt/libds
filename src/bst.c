#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int data;
    struct Node* left;
    struct Node* right;
}Node;


Node* create_node(int val){
    Node* new_node = malloc(sizeof(Node));
    new_node->data = val;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

Node* insert(Node* root,int val){
    if(root == NULL){
        return create_node(val);
    }

    if(val<root->data){
        root->left = insert(root->left,val);
    }else if(val>root->data){
        root->right = insert(root->right,val);
    }

    return root;
}

Node* search(Node *root, int val){
    if (root == NULL) {
        return NULL;
    }

    if (val == root->data) {
        return root;
    }
    else if (val < root->data) {
        return search(root->left, val);
    }
    else {
        return search(root->right, val);
    }
}


Node *find_min(Node *root){
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}

Node* delete(Node* root,int val){
    if(root == NULL){
        return NULL;
    }

    if(val<root->data){
        root->left = delete(root->left,val);
    }else if(val>root->data){
        root->right = delete(root->right,val);
    }else{
        //we found the node to delete 
        if(root->left == NULL && root->right ==NULL){
            free(root);
            return NULL;
        }
        if(root->left == NULL){
            Node* temp = root->right;
            free(root);
            return temp;
        }
        if(root->left == NULL){
            Node* temp = root->left;
            free(root);
            return temp;
        }

        //if both child exists
        Node* successor = find_min(root->right); //find min from right side
        root->data = successor->data; // copy the value 
        root->right = delete(root->right,successor->data); //delete the sucesoor from right side

    }

    return root;
}
void destroy_tree(Node *root){
    if (root == NULL) {
        return;
    }

    destroy_tree(root->left);
    destroy_tree(root->right);

    free(root);
}


void inorder(Node *root){
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main(void)
{
    Node *root = NULL;

    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("BST: ");
    inorder(root);
    printf("\n");

    Node *result = search(root, 40);

    if (result != NULL) {
        printf("Found: %d\n", result->data);
    }
    else {
        printf("Not found\n");
    }

    printf("Deleting 20...\n");
    root = delete_node(root, 20);

    printf("BST: ");
    inorder(root);
    printf("\n");

    printf("Deleting 30...\n");
    root = delete_node(root, 30);

    printf("BST: ");
    inorder(root);
    printf("\n");

    printf("Deleting 50...\n");
    root = delete_node(root, 50);

    printf("BST: ");
    inorder(root);
    printf("\n");

    destroy_tree(root);

    return 0;
}

