#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} node;

typedef struct {
    node* head;
    size_t length;
} list;

list l;

void list_init(void) {
    l.head = NULL;
    l.length = 0;
}

int list_push_front(int val) {
    node* n = malloc(sizeof(node));
    if (n == NULL) {
        return 1; // Allocation failure
    }
    n->data = val;
    n->next = l.head;
    l.head = n;
    l.length++;
    return 0;
}

int list_push_back(int val) {
    node* n = malloc(sizeof(node));
    if (n == NULL) {
        return 1;
    }
    n->data = val;
    n->next = NULL;

    // Handle empty list
    if (l.head == NULL) {
        l.head = n;
        l.length++;
        return 0;
    }

    node* curr;
    for (curr = l.head; curr->next != NULL; curr = curr->next) {
        // get to last position
    }
    curr->next = n;
    l.length++;
    return 0;
}

int list_find(int val, node** out) {
    if (out == NULL) {
        return 1;
    }
    node* curr = l.head;
    while (curr != NULL && curr->data != val) {
        curr = curr->next;
    }
    if (curr == NULL) {
        return 1;
    }
    *out = curr;
    return 0;
}

int list_destroy(void) {
    node* curr = l.head;
    while (curr != NULL) {
        node* aux = curr;
        curr = curr->next;
        free(aux);
    }
    l.head = NULL;
    l.length = 0;
    return 0;
}

int list_insert_at(int val, int index) {
    if (index > l.length) {
        printf("u have given index which doesnt exist\n");
        return 1;
    }
    if (index == 0) {
        return list_push_front(val);
    }

    node* n = malloc(sizeof(node));
    if (n == NULL) {
        return 1;
    }

    node* curr = l.head;
    int pos = 1;
    while (pos != index) {
        curr = curr->next;
        pos++;
    }
    
    n->data = val;
    n->next = curr->next;
    curr->next = n;
    l.length++;
    return 0;
}

int list_remove_front(void) {
    if (l.head == NULL) {
        printf("list is empty\n");
        return 1;
    }
    node* temp = l.head;
    l.head = l.head->next;
    free(temp);
    l.length--;
    return 0;
}

int list_remove_back(void) {
    if (l.head == NULL) {
        printf("list is empty\n");
        return 1;
    }
    if (l.head->next == NULL) {
        free(l.head);
        l.head = NULL;
        l.length--;
        return 0;
    }
    node* curr = l.head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    free(curr->next);
    curr->next = NULL;
    l.length--;
    return 0;
}

int list_remove_at(int index) {
    if (index >= l.length || l.head == NULL) {
        printf("index out of bounds\n");
        return 1;
    }
    if (index == 0) {
        return list_remove_front();
    }
    node* curr = l.head;
    int pos = 1;
    while (pos != index) {
        curr = curr->next;
        pos++;
    }
    node* temp = curr->next;
    curr->next = temp->next;
    free(temp);
    l.length--;
    return 0;
}

int list_get(int index, int* out) {
    if (out == NULL || index >= l.length || l.head == NULL) {
        printf("index out of bounds or invalid pointer\n");
        return 1;
    }
    node* curr = l.head;
    int pos = 0;
    while (pos != index) {
        curr = curr->next;
        pos++;
    }
    *out = curr->data;
    return 0;
}

int list_set(int index, int val) {
    if (index >= l.length || l.head == NULL) {
        printf("index out of bounds\n");
        return 1;
    }
    node* curr = l.head;
    int pos = 0;
    while (pos != index) {
        curr = curr->next;
        pos++;
    }
    curr->data = val;
    return 0;
}

node* list_reverse(void) {
    node* prev = NULL;
    node* curr = l.head;
    node* next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    l.head = prev;
    return l.head;
}

int list_insert_sorted(int val) {
    if (l.head == NULL || val <= l.head->data) {
        return list_push_front(val);
    }

    node* n = malloc(sizeof(node));
    if (n == NULL) {
        return 1;
    }
    n->data = val;

    node* curr = l.head;
    while (curr->next != NULL && curr->next->data < val) {
        curr = curr->next;
    }

    n->next = curr->next;
    curr->next = n;
    l.length++;
    return 0;
}

void list_print(void) {
    for (node* curr = l.head; curr != NULL; curr = curr->next) {
        printf("%d\n", curr->data); 
    }
}

int main(void) {
    list_init(); // Initializes head to NULL and length to 0

    list_push_front(1);
    list_push_front(2);
    list_push_front(3);
    list_push_back(0);
    
    node* find;    
    if (list_find(2, &find) != 1) {
        printf("found the node\n");
        printf("its value is %d\n", find->data);
    }
    list_insert_at(10, 1);

    printf("--- After insertions ---\n");
    list_print();

    printf("--- Testing list_set & list_get ---\n");
    list_set(2, 99);
    int val;
    if (list_get(2, &val) == 0) {
        printf("Value at index 2 is: %d\n", val);
    }

    printf("--- Testing removals ---\n");
    list_remove_front();
    list_remove_back();
    
    printf("--- Final print ---\n");
    list_print();

    // Clean up heap memory properly
    list_destroy();

    return 0;
}
