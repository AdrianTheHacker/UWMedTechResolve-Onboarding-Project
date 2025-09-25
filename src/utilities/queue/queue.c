#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int queue_print(queue_t *queue) {
  printf(" Queue: [");
  for(int i = 0; i < QUEUE_LENGTH - 1; i++) {
    printf("%lf, ", queue->buffer[i]);
  }
  printf("%lf]\n", queue->buffer[QUEUE_LENGTH - 1]);

  return 0;
}

int queue_initialize(queue_t *queue) {
  queue->head = 0;
  queue->tail = 0;

  return 0;
}

int queue_push(queue_t *queue, double value) {
  if(queue->tail + 1 == queue->head) return QUEUE_FULL_ERROR;
  if(queue->tail == QUEUE_LENGTH && queue->head == 0) return QUEUE_FULL_ERROR;

  if(queue->tail == QUEUE_LENGTH) {
    queue->buffer[queue->tail] = value;
    queue->tail = 0;
    return QUEUE_OPERATION_SUCESSFUL;
  }

  queue->buffer[queue->tail] = value;
  queue->tail++;

  return 0;
}

int queue_pop(queue_t *queue, double *value) {
  if(queue->tail == queue->head) return QUEUE_EMPTY_ERROR;

  *value = queue->buffer[queue->head];
  queue->buffer[queue->head] = 0;

  if(queue->head == QUEUE_LENGTH) {
    queue->head = 0;
    return 0;
  }

  queue->head++;
  return 0;
}

int queue_populate_buffer(queue_t *queue, double *buffer, int buffer_length) {
  for(int i = 0; i < buffer_length; i++) {
    if(queue_pop(queue, &buffer[i]) == QUEUE_EMPTY_ERROR) return QUEUE_EMPTY_ERROR;
  }

  return QUEUE_OPERATION_SUCESSFUL;
}