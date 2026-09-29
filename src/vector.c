
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct vector {
    int *data;
    size_t length;
    size_t capacity;
} vector;


// Initialize an empty vector
void vector_init(vector *v)
{
    v->data = NULL;
    v->length = 0;
    v->capacity = 0;
}


// Increase capacity when needed
int vector_reserve(vector *v, size_t min_capacity){
    if (min_capacity <= v->capacity) {
        return 0;
    }

    size_t new_capacity = v->capacity;

    if (new_capacity == 0) {
        new_capacity = 4;
    }

    while (new_capacity < min_capacity) {
        // Prevent overflow when doubling capacity
        if (new_capacity > SIZE_MAX / 2) {
            return -1;
        }

        new_capacity *= 2;
    }

    // Ensure allocation size will not overflow
    if (new_capacity > SIZE_MAX / sizeof(int)) {
        return -1;
    }

    int *new_data = realloc(
        v->data,
        new_capacity * sizeof(int)
    );

    if (new_data == NULL) {
        return -1;
    }

    v->data = new_data;
    v->capacity = new_capacity;

    return 0;
}


// Append an element to the end
int vector_push(vector *v, int value){
    if (v->length == v->capacity) {
        size_t new_capacity = 4;

        if (v->capacity != 0) {
            if (v->capacity > SIZE_MAX / 2) {
                return -1;
            }

            new_capacity = v->capacity * 2;
        }

        if (vector_reserve(v, new_capacity) != 0) {
            return -1;
        }
    }

    v->data[v->length] = value;
    v->length++;

    return 0;
}


// Remove the last element
int vector_pop(vector *v, int *out){
    if (v->length == 0) {
        //theres nothing to pop
        return -1;
    }

    v->length--;

    if (out != NULL) {
        *out = v->data[v->length];
    }

    return 0;
}


// Read an element at an index
int vector_get(const vector *v, size_t index, int *out){
    if (index >= v->length || out == NULL) {
        return -1;
    }

    *out = v->data[index];

    return 0;
}


// Modify an existing element
int vector_set(vector *v, size_t index, int value){
    if (index >= v->length) {
        return -1;
    }

    v->data[index] = value;

    return 0;
}


// Free the vector's allocated memory
void vector_destroy(vector *v){
    free(v->data);

    v->data = NULL;
    v->length = 0;
    v->capacity = 0;
}


// Test the vector
int main(void)
{
    vector v;
    vector_init(&v);

    // Add elements
    for (int i = 1; i <= 10; i++) {
        if (vector_push(&v, i * 10) != 0) {
            printf("Push failed\n");
            vector_destroy(&v);
            return 1;
        }
    }

    printf("Length: %zu\n", v.length);
    printf("Capacity: %zu\n", v.capacity);

    // Read an element
    int value;

    if (vector_get(&v, 2, &value) == 0) {
        printf("Element at index 2: %d\n", value);
    }

    // Modify an element
    vector_set(&v, 2, 999);

    vector_get(&v, 2, &value);
    printf("After set: %d\n", value);

    // Remove the last element
    if (vector_pop(&v, &value) == 0) {
        printf("Popped: %d\n", value);
    }

    printf("Length after pop: %zu\n", v.length);

    // Test an invalid index
    if (vector_get(&v, 100, &value) != 0) {
        printf("Invalid index handled correctly\n");
    }

    // Print all remaining elements
    printf("Vector: ");

    for (size_t i = 0; i < v.length; i++) {
        vector_get(&v, i, &value);
        printf("%d ", value);
    }

    printf("\n");

    // Release allocated memory
    vector_destroy(&v);

    return 0;
}
