#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#include "../utilities/queue/queue.h"

#define REQUEST_LENGTH 5
#define AMOUNT_OF_ELEMENTS_IN_DATA_PACKET 5
#define READ_TIME_PERIOD_IN_SECONDS 10
#define GET_REQUEST "GET"
#define STOP_REQUEST "STOP"

#define handle_error(message) \
  do { perror(message); exit(EXIT_FAILURE); } while(0)

#define DATA_NONEXISTENT_ERROR -51

void *unix_socket_interface_thread_routine(void *state);
void *file_handler_thread_routine(void *state);

int check_data_existent(double *buffer, int buffer_length);
void print_buffer(double *buffer, int buffer_length);

queue_t queue;
pthread_mutex_t mutex;
FILE *fd;

int main() {
  fd = fopen("./database/sensor_log.txt", "a");
  queue_initialize(&queue);
  pthread_mutex_init(&mutex, NULL);

  pthread_t unix_socket_interface_thread;
  pthread_t file_handler_thread;

  pthread_create(&unix_socket_interface_thread, NULL, &unix_socket_interface_thread_routine, NULL);
  pthread_create(&file_handler_thread, NULL, &file_handler_thread_routine, NULL);

  pthread_join(file_handler_thread, NULL);
  pthread_join(unix_socket_interface_thread, NULL);
  
  pthread_mutex_destroy(&mutex);
  fclose(fd);
  return 0;
}

void *unix_socket_interface_thread_routine(void *state) {
  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_fd == -1) { handle_error("socket"); }

  const struct sockaddr_un server_address = {
    .sun_family = AF_UNIX,
    .sun_path = "/tmp/sensor_socket"
  };

  if(connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    handle_error("connect");
  }

  for(int i = 0; i < 20; i++) {
    int write_result_status = write(server_fd, GET_REQUEST, strlen(GET_REQUEST));
    if (write_result_status <= 0) { sleep(READ_TIME_PERIOD_IN_SECONDS); continue; }

    double data_packet_buffer[AMOUNT_OF_ELEMENTS_IN_DATA_PACKET];

    int read_result_status = read(server_fd, data_packet_buffer, sizeof(data_packet_buffer));
    if (read_result_status <= 0) { sleep(READ_TIME_PERIOD_IN_SECONDS); continue; }

    print_buffer(data_packet_buffer, AMOUNT_OF_ELEMENTS_IN_DATA_PACKET);

    if (check_data_existent(data_packet_buffer, AMOUNT_OF_ELEMENTS_IN_DATA_PACKET) == -1) {
      sleep(READ_TIME_PERIOD_IN_SECONDS);
      continue;
    }

    pthread_mutex_lock(&mutex);
    for(int j = 0; j < AMOUNT_OF_ELEMENTS_IN_DATA_PACKET; j++) {
      int queue_operation_result = queue_push(&queue, data_packet_buffer[j]);
      if (queue_operation_result == QUEUE_OPERATION_SUCESSFUL) continue;

      double outdated_queue_value;
      queue_pop(&queue, &outdated_queue_value);
      queue_push(&queue, data_packet_buffer[j]);
    }
    pthread_mutex_unlock(&mutex);

    pthread_mutex_lock(&mutex);
    queue_print(&queue);
    pthread_mutex_unlock(&mutex);

    sleep(READ_TIME_PERIOD_IN_SECONDS);
  }

  write(server_fd, STOP_REQUEST, strlen(STOP_REQUEST));
  exit(EXIT_SUCCESS);
}

void *file_handler_thread_routine(void *state) {
  while(1) {
    double reading;

    pthread_mutex_lock(&mutex);
    int queue_status = queue_pop(&queue, &reading);
    pthread_mutex_unlock(&mutex);

    if (queue_status == QUEUE_EMPTY_ERROR) {
      reading = -51;
    }
    
    printf("Temperature Reading: %f Celsius\n", reading);

    fflush(fd);
    fprintf(fd, "Temperature Reading: %f Celsius\n", reading);

    sleep(2);
  }
}

int check_data_existent(double *buffer, int buffer_length) {
  for (int i = 0; i < buffer_length; i++) {
    if (buffer[i] == DATA_NONEXISTENT_ERROR) return -1;
  }

  return 0;
}

void print_buffer(double *buffer, int buffer_length) {
  printf("Buffer: {");
  for(int i = 0; i < buffer_length - 1; i++) {
    printf("%lf, ", buffer[i]);
  }
  printf("%lf}\n", buffer[buffer_length - 1]);
}