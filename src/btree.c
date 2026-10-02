#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct Node {
    int keys[2 * T - 1];
    struct Node *children[2 * T];

    int key_count;
    int is_leaf;
} Node;


/* Create a new B-tree node */
Node *create_node(int is_leaf){
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        return NULL;
    }

    new_node->key_count = 0;
    new_node->is_leaf = is_leaf;

    for (int i = 0; i < 2 * T; i++) {
        new_node->children[i] = NULL;
    }

    return new_node;
}


/* Search for a value */
Node *search(Node *node, int value){
    if (node == NULL) {
        return NULL;
    }

    int i = 0;

    /*
     * Find the first key that is
     * greater than or equal to value.
     */
    while (i < node->key_count && value > node->keys[i]) {
        i++;
    }

    /*
     * We found the value.
     */
    if (i < node->key_count && value == node->keys[i]) {
        return node;
    }

    /*
     * No more children to search.
     */
    if (node->is_leaf) {
        return NULL;
    }

    /*
     * Search the appropriate child.
     */
    return search(node->children[i], value);
}


/*
 * Split a full child of parent.
 *
 * Before:
 *
 *             parent
 *                |
 *          [5 | 10 | 15]
 *
 * After:
 *
 *             [10]
 *            /    \
 *          [5]    [15]
 */
void split_child(Node *parent, int index){
    Node *full_child = parent->children[index];

    Node *new_child = create_node(full_child->is_leaf);

    if (new_child == NULL) {
        return;
    }

    /*
     * Save the middle key before
     * changing the child.
     */
    int middle_key = full_child->keys[T - 1];

    /*
     * Move the keys to the right of
     * the middle key into new_child.
     */
    new_child->key_count = T - 1;

    for (int j = 0; j < T - 1; j++) {
        new_child->keys[j] =
            full_child->keys[j + T];
    }

    /*
     * If this is an internal node,
     * move the right-side children too.
     */
    if (!full_child->is_leaf) {
        for (int j = 0; j < T; j++) {
            new_child->children[j] =
                full_child->children[j + T];
        }
    }

    /*
     * The original child keeps only
     * the keys to the left of middle_key.
     */
    full_child->key_count = T - 1;

    /*
     * Move parent's children to the right
     * to make room for new_child.
     */
    for (int j = parent->key_count; j >= index + 1; j--) {
        parent->children[j + 1] =
            parent->children[j];
    }

    /*
     * Put new_child immediately after
     * full_child.
     */
    parent->children[index + 1] = new_child;

    /*
     * Move parent's keys to the right
     * to make room for middle_key.
     */
    for (int j = parent->key_count - 1; j >= index; j--) {
        parent->keys[j + 1] =
            parent->keys[j];
    }

    /*
     * Promote the middle key.
     */
    parent->keys[index] = middle_key;
    parent->key_count++;
}


/*
 * Insert into a node that is guaranteed
 * to be non-full.
 */
void insert_non_full(Node *node, int value){
    int i = node->key_count - 1;

    /*
     * Case 1:
     * Node is a leaf.
     */
    if (node->is_leaf) {

        /*
         * Shift larger keys to the right.
         */
        while (i >= 0 && value < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        /*
         * Insert value into the
         * newly-created space.
         */
        node->keys[i + 1] = value;
        node->key_count++;

        return;
    }

    /*
     * Case 2:
     * Node is an internal node.
     *
     * Find the child where value belongs.
     */
    while (i >= 0 && value < node->keys[i]) {
        i--;
    }

    i++;

    /*
     * If the child is full, split it
     * before descending.
     */
    if (node->children[i]->key_count == 2 * T - 1) {

        split_child(node, i);

        /*
         * split_child() promoted a key
         * into node.
         *
         * Decide whether value belongs
         * to the left or right child.
         */
        if (value > node->keys[i]) {
            i++;
        }
    }

    /*
     * The selected child is guaranteed
     * to be non-full now.
     */
    insert_non_full(node->children[i], value);
}


/*
 * Top-level B-tree insertion.
 */
Node *insert(Node *root, int value){
    /*
     * Empty tree.
     */
    if (root == NULL) {
        root = create_node(1);

        if (root == NULL) {
            return NULL;
        }

        root->keys[0] = value;
        root->key_count = 1;

        return root;
    }

    /*
     * If root is full, we must create
     * a new root before descending.
     */
    if (root->key_count == 2 * T - 1) {

        Node *new_root = create_node(0);

        if (new_root == NULL) {
            return root;
        }

        /*
         * Old root becomes the first child
         * of the new root.
         */
        new_root->children[0] = root;

        /*
         * Split the old root.
         */
        split_child(new_root, 0);

        /*
         * Decide which child should receive
         * the new value.
         */
        int i = 0;

        if (value > new_root->keys[0]) {
            i++;
        }

        insert_non_full(new_root->children[i], value);

        return new_root;
    }

    /*
     * Root has space, so insert normally.
     */
    insert_non_full(root, value);

    return root;
}


/* Display the B-tree */
void display(Node *node, int depth){
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("    ");
    }

    printf("[");

    for (int i = 0; i < node->key_count; i++) {
        printf("%d", node->keys[i]);

        if (i + 1 < node->key_count) {
            printf(" | ");
        }
    }

    printf("]\n");

    if (!node->is_leaf) {
        for (int i = 0; i <= node->key_count; i++) {
            display(node->children[i], depth + 1);
        }
    }
}


/* Destroy the entire B-tree */
void destroy_tree(Node *root){
    if (root == NULL) {
        return;
    }

    if (!root->is_leaf) {
        for (int i = 0; i <= root->key_count; i++) {
            destroy_tree(root->children[i]);
        }
    }

    free(root);
}


int main(void)
{
    Node *root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 12);
    root = insert(root, 30);
    root = insert(root, 7);
    root = insert(root, 17);

    printf("B-tree:\n");
    display(root, 0);

    printf("\nSearch:\n");

    if (search(root, 12) != NULL) {
        printf("12 found\n");
    }
    else {
        printf("12 not found\n");
    }

    if (search(root, 25) != NULL) {
        printf("25 found\n");
    }
    else {
        printf("25 not found\n");
    }

    destroy_tree(root);

    return 0;
}

