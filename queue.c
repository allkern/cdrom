#include <stdlib.h>
#include <string.h>

#include "queue.h"

queue_t* queue_create(void) {
    return malloc(sizeof(queue_t));
}

void queue_init(queue_t* queue, size_t size) {
    queue->buf = malloc(size);
    queue->read_index = 0;
    queue->write_index = 0;
    queue->size = size;
}

void queue_push(queue_t* queue, uint8_t value) {
    if (queue_is_full(queue))
        return;

    queue->buf[queue->write_index++] = value;
}

uint8_t queue_pop(queue_t* queue) {
    if (queue_is_empty(queue))
        return 0;

    return queue->buf[queue->read_index++];
}

uint8_t queue_peek(queue_t* queue) {
    if (queue_is_empty(queue))
        return 0;

    return queue->buf[queue->read_index];
}

int queue_is_empty(queue_t* queue) {
    return queue->read_index == queue->write_index;
}

int queue_is_full(queue_t* queue) {
    return queue->write_index == queue->size;
}

void queue_reset(queue_t* queue) {
    queue->write_index = 0;
    queue->read_index = 0;
}