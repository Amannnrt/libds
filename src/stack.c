
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *data;
    size_t length;
    size_t capacity;
} stack;


// Initialize an empty stack
void stack_init(stack *s){
    s->data = NULL;
    s->length = 0;
    s->capacity = 0;
}


// Add an element to the top
int stack_push(stack *s, int value){
    // Grow the array if it is full
    if (s->length == s->capacity) {
        size_t new_capacity = 4;

        if (s->capacity != 0) {
            new_capacity = s->capacity * 2;
        }

        int *new_data = realloc(
            s->data,
            new_capacity * sizeof(int)
        );

        if (new_data == NULL) {
            return -1;
        }

        s->data = new_data;
        s->capacity = new_capacity;
    }

    // Place the new value at the top
    s->data[s->length] = value;
    s->length++;

    return 0;
}


// Remove the top element
int stack_pop(stack *s, int *out){
    if (s->length == 0) {
        return -1;
    }

    s->length--;

    if (out != NULL) {
        *out = s->data[s->length];
    }

    return 0;
}


// Read the top element without removing it
int stack_peek(const stack *s, int *out){
    if (s->length == 0 || out == NULL) {
        return -1;
    }

    *out = s->data[s->length - 1];

    return 0;
}


// Free the stack's allocated memory
void stack_destroy(stack *s){
    free(s->data);

    s->data = NULL;
    s->length = 0;
    s->capacity = 0;
}


int main(void)
{
    stack s;
    stack_init(&s);

    stack_push(&s, 10);
    stack_push(&s, 20);
    stack_push(&s, 30);

    int value;

    if (stack_peek(&s, &value) == 0) {
        printf("Top element: %d\n", value);
    }

    if (stack_pop(&s, &value) == 0) {
        printf("Popped: %d\n", value);
    }

    if (stack_peek(&s, &value) == 0) {
        printf("New top: %d\n", value);
    }

    printf("Stack length: %zu\n", s.length);
    printf("Stack capacity: %zu\n", s.capacity);

    stack_destroy(&s);

    return 0;
}
