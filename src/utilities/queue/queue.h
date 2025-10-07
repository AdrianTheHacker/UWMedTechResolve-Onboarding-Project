#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_LENGTH 20
#define READING_SIZE 5

#define QUEUE_FULL_ERROR -1
#define QUEUE_EMPTY_ERROR -1
#define QUEUE_OPERATION_SUCESSFUL 0

#define PRINT_FULL_QUEUE_CMD 10
#define PRINT_FUNCTIONAL_QUEUE_CMD 11

typedef struct queue_t {
  double buffer[QUEUE_LENGTH + 1];
  int head;
  int tail;
  int length;
} queue_t;

int queue_print(queue_t *queue);
int queue_initialize(queue_t *queue);
int queue_push(queue_t *queue, double value);
int queue_pop(queue_t *queue, double *value);
int queue_populate_buffer(queue_t *queue, double *buffer, int buffer_length);

#endif