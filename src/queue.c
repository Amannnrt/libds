#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;

    size_t length;
    size_t capacity;
    size_t front;
} queue;


/* Initialize queue */
int queue_init(queue *q){
    
    q->capacity = 4;
    q->length = 0;
    q->front = 0;

    q->data = malloc(q->capacity * sizeof(int));

    if (q->data == NULL) {
        return -1;
    }

    return 0;
}


//add element
int queue_enqueue(queue *q, int value)
{
    /* Queue is full -> grow */
    if (q->length == q->capacity) {
        
        size_t new_capacity = q->capacity * 2;
        int *new_data = malloc(new_capacity * sizeof(int));
        
        if (new_data == NULL) {
            return -1;
        }

        /*
         * Copy elements in logical FIFO order.
         *
         * We cannot simply copy data[0], data[1], ...
         * because the queue might be wrapped.
         */
        for (size_t i = 0; i < q->length; i++) {
            size_t index = (q->front + i) % q->capacity;
            new_data[i] = q->data[index];
        }

        free(q->data);

        q->data = new_data;
        q->capacity = new_capacity;

        /* Elements now start at index 0 */
        q->front = 0;
    }

    /*
     * Find the position after the last element.
     */
    size_t rear = (q->front + q->length) % q->capacity;

    q->data[rear] = value;
    q->length++;

    return 0;
}


//Remove front element 
int queue_dequeue(queue *q, int *out){
    if (q->length == 0) {
        return -1;
    }

    *out = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->length--;

    return 0;
}


//Look at front element 
int queue_peek(const queue *q, int *out){
    if (q->length == 0) {
        return -1;
    }
    *out = q->data[q->front];

    return 0;
}


// Print queue in FIFO order 
void queue_print(const queue *q){
    for (size_t i = 0; i < q->length; i++) {
        size_t index = (q->front + i) % q->capacity;
        printf("%d ", q->data[index]);
    }

    printf("\n");
}


//free queue
void queue_destroy(queue *q)
{
    free(q->data);

    q->data = NULL;
    q->length = 0;
    q->capacity = 0;
    q->front = 0;
}


int main(void){
    queue q;
    if (queue_init(&q) != 0) {
        printf("Failed to initialize queue\n");
        return 1;
    }

    queue_enqueue(&q, 10);
    queue_enqueue(&q, 20);
    queue_enqueue(&q, 30);
    queue_enqueue(&q, 40);

    queue_print(&q);


    int value;

    queue_dequeue(&q, &value);
    printf("dequeued: %d\n", value);

    queue_dequeue(&q, &value);
    printf("dequeued: %d\n", value);


    queue_enqueue(&q, 50);
    queue_enqueue(&q, 60);

    queue_print(&q);


    /* This will cause the queue to grow */
    queue_enqueue(&q, 70);

    queue_print(&q);

    printf("length: %zu\n", q.length);
    printf("capacity: %zu\n", q.capacity);


    queue_destroy(&q);

    return 0;
}

