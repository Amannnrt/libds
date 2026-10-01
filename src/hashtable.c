#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* next;
} Node;

typedef struct Hashtable {
    Node** ht;
    int cap;
} Hashtable;


void ht_init(Hashtable* hashtable, int cap) {
    hashtable->cap = cap;
    hashtable->ht = malloc(cap * sizeof(Node*));
    
    for (int i = 0; i < cap; i++) {
        hashtable->ht[i] = NULL;
    }
}

// Hash function 
int hash(int val, int cap) {
    return val % cap;
}

// Add value to hash table
void hash_add(Hashtable* hashtable, int val) {
    int index = hash(val, hashtable->cap);
    if (hashtable->ht[index] == NULL) {
        hashtable->ht[index] = malloc(sizeof(Node));
        hashtable->ht[index]->key = val;
        hashtable->ht[index]->next = NULL;
        printf("Value %d added (new bucket)\n", val);
    } else {
        Node* new_node = malloc(sizeof(Node));
        new_node->key = val;
        new_node->next = hashtable->ht[index];
        hashtable->ht[index] = new_node;
        printf("Value %d added after collision handling\n", val);
    }
}

// Find function
int hash_find(Hashtable* hashtable, int val) {
    int index = hash(val, hashtable->cap);
    Node* current = hashtable->ht[index];
    
    while (current != NULL) {
        if (current->key == val) {
            return 1; // Found
        }
        current = current->next;
    }
    return 0; // Not found
}

// Remove function
void hash_remove(Hashtable* hashtable, int val) {
    int index = hash(val, hashtable->cap);
    Node* current = hashtable->ht[index];
    Node* prev = NULL;

    while (current != NULL) {
        if (current->key == val) {
            if (prev == NULL) {
                hashtable->ht[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Value %d removed successfully\n", val);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Value %d not found for removal\n", val);
}

// Destroy function (frees nodes AND the table array itself)
void ht_destroy(Hashtable* hashtable) {
    for (int i = 0; i < hashtable->cap; i++) {
        Node* current = hashtable->ht[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
    // Free the array of pointers itself
    free(hashtable->ht);
    hashtable->ht = NULL;
    hashtable->cap = 0;
    printf("Hashtable destroyed and all memory freed successfully\n");
}

int main() {
    Hashtable hashtable;
    
    int user_capacity = 5;
    ht_init(&hashtable, user_capacity);

    // Adding elements
    hash_add(&hashtable, 2);
    hash_add(&hashtable, 7);  
    hash_add(&hashtable, 42); 

    // Testing find
    printf("\n--- Testing Find ---\n");
    printf("Is 7 in hash table? %s\n", hash_find(&hashtable, 7) ? "Yes" : "No");

    // Testing remove
    printf("\n--- Testing Remove ---\n");
    hash_remove(&hashtable, 7); 
    printf("Is 7 still in hash table? %s\n", hash_find(&hashtable, 7) ? "Yes" : "No");

    // Clean up memory
    printf("\n--- Cleanup ---\n");
    ht_destroy(&hashtable);

    return 0;
}
