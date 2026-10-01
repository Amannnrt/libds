#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;

    size_t capacity;
    size_t length;

    size_t head;    // where we read
    size_t tail;    // where we write
} ring_buffer;


// Initialize the ring buffer
int ring_buffer_init(ring_buffer *rb, size_t capacity)
{
    rb->data = malloc(capacity * sizeof(int));
    if (rb->data == NULL) {
        return -1;
 

    rb->capacity = capacity;
    rb->length = 0;

    rb->head = 0;
    rb->tail = 0;

    return 0;
}


/*
 * Write an element into the ring buffer
 */
int ring_buffer_write(ring_buffer *rb, int value){
    /*
     * Cannot write if the buffer is full.
     */
    if (rb->length == rb->capacity) {
        return -1;
    }

    rb->data[rb->tail] = value;
    /*
     * Move tail forward.
     * % capacity makes it wrap around.
     */
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->length++;

    return 0;
}


//read the oldest element
int ring_buffer_read(ring_buffer *rb, int *out){
    //nothing to read
    if (rb->length == 0) {
        return -1;
    }

    *out = rb->data[rb->head];

    //move head forward 
    rb->head = (rb->head + 1) % rb->capacity;
    rb->length--;

    return 0;
}

// Look at the oldest element without removing it.
int ring_buffer_peek(const ring_buffer *rb, int *out){
    if (rb->length == 0) {
        return -1;
    }

    *out = rb->data[rb->head];

    return 0;
}

// Print elements in logical order.
void ring_buffer_print(const ring_buffer *rb){
    for (size_t i = 0; i < rb->length; i++) {
        size_t index = (rb->head + i) % rb->capacity;
        printf("%d ", rb->data[index]);
    }
    printf("\n");
}


//free the ring buffer
void ring_buffer_destroy(ring_buffer *rb)
{
    free(rb->data);

    rb->data = NULL;
    rb->capacity = 0;
    rb->length = 0;
    rb->head = 0;
    rb->tail = 0;
}


int main(void){
    ring_buffer rb;

    if (ring_buffer_init(&rb, 5) != 0) {
        printf("Failed to initialize ring buffer\n");
        return 1;
    }


    printf("Writing 10, 20, 30\n");

    ring_buffer_write(&rb, 10);
    ring_buffer_write(&rb, 20);
    ring_buffer_write(&rb, 30);

    ring_buffer_print(&rb);


    int value;

    printf("Reading: ");

    ring_buffer_read(&rb, &value);

    printf("%d\n", value);


    printf("Writing 40, 50, 60\n");

    ring_buffer_write(&rb, 40);
    ring_buffer_write(&rb, 50);
    ring_buffer_write(&rb, 60);

    ring_buffer_print(&rb);


    printf("Head: %zu\n", rb.head);
    printf("Tail: %zu\n", rb.tail);
    printf("Length: %zu\n", rb.length);
    printf("Capacity: %zu\n", rb.capacity);


    printf("Trying to write 70...\n");

    if (ring_buffer_write(&rb, 70) != 0) {
        printf("Buffer is full\n");
    }


    ring_buffer_destroy(&rb);

    return 0;
}
