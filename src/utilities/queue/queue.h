#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_LENGTH 50
#define READING_SIZE 5

#define QUEUE_FULL_ERROR -1
#define QUEUE_EMPTY_ERROR -1
#define QUEUE_OPERATION_SUCESSFUL 0

typedef struct queue_t {
  double buffer[QUEUE_LENGTH + 1];
  int head;
  int tail;
} queue_t;

int queue_print(queue_t *queue);
int queue_initialize(queue_t *queue);
int queue_push(queue_t *queue, double value);
int queue_pop(queue_t *queue, double *value);
int queue_populate_buffer(queue_t *queue, double *buffer, int buffer_length);

#endif