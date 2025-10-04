#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#include "drivers/temperature_sensor/temperature_sensor.h"
#include "../utilities/queue/queue.h"

#define REQUEST_LENGTH 5
#define DATA_PACKET_LENGTH_IN_BYTES 40
#define handle_error(message) \
  do { perror(message); exit(EXIT_FAILURE); } while(0)

#define DATA_NONEXISTENT_ERROR -51

#define GET_REQUEST "GET"
#define STOP_REQUEST "STOP"

void *unix_socket_interface_thread_routine(void *state);
void *temperature_sensor_simulation_thread_routine(void *state);

queue_t queue;
pthread_mutex_t mutex;

int main() {
  queue_initialize(&queue);
  pthread_mutex_init(&mutex, NULL);
  pthread_t unix_socket_interface_thread;
  pthread_t temperature_sensor_simulation_thread;

  pthread_create(&unix_socket_interface_thread, NULL, &unix_socket_interface_thread_routine, NULL);
  pthread_create(&temperature_sensor_simulation_thread, NULL, &temperature_sensor_simulation_thread_routine, NULL);

  pthread_join(temperature_sensor_simulation_thread, NULL);
  pthread_join(unix_socket_interface_thread, NULL);
  
  pthread_mutex_destroy(&mutex);
  return 0;
}

void *unix_socket_interface_thread_routine(void *state) {
  (void)state;

  int my_fd;
  int peer_fd;

  const struct sockaddr_un my_address = {
    .sun_family = AF_UNIX,
    .sun_path = "/tmp/sensor_socket"
  };
  struct sockaddr_un peer_address;

  unlink(my_address.sun_path);
  my_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (my_fd == -1) { handle_error("socket"); }

  if (bind(my_fd, (struct sockaddr *)&my_address, sizeof(my_address)) == -1) {
    handle_error("bind");
  }

  if (listen(my_fd, 2) == -1) {
    handle_error("listen");
  }

  socklen_t peer_address_size = sizeof(peer_address);
  peer_fd = accept(my_fd, (struct sockaddr *)&peer_address, &peer_address_size);

  if (peer_fd == -1) {
    handle_error("accept");
  }

  char request_buffer[REQUEST_LENGTH];
  double data_packet_buffer[DATA_PACKET_LENGTH_IN_BYTES / 8];
  
  while (1) {
    if (read(peer_fd, request_buffer, REQUEST_LENGTH) <= 0) continue;
    printf("Incoming Request: %s\n", request_buffer);

    if (strncmp(request_buffer, GET_REQUEST, strlen(GET_REQUEST)) == 0) {
      pthread_mutex_lock(&mutex);
      int queue_operation_result = queue_populate_buffer(&queue, data_packet_buffer, DATA_PACKET_LENGTH_IN_BYTES);
      pthread_mutex_unlock(&mutex);

      if (queue_operation_result == QUEUE_EMPTY_ERROR) {
        for(int i = 0; i < DATA_PACKET_LENGTH_IN_BYTES; i++) { data_packet_buffer[i] = DATA_NONEXISTENT_ERROR; }
      }

      write(peer_fd, data_packet_buffer, DATA_PACKET_LENGTH_IN_BYTES);
    }

    if (strncmp(request_buffer, STOP_REQUEST, strlen(STOP_REQUEST)) == 0) {
      break;
    }
  }
  
  if (close(peer_fd) == -1) {
    handle_error("close");
  }

  if (close(my_fd) == -1) {
    handle_error("close");
  }

  unlink(my_address.sun_path);
  exit(EXIT_SUCCESS);
}

void *temperature_sensor_simulation_thread_routine(void *state) {
  while(1) {
    double temperature_sensor_reading_in_celsius = get_temperature_in_degrees_celsius();

    pthread_mutex_lock(&mutex);
    int queue_operation_results = queue_push(&queue, temperature_sensor_reading_in_celsius);
    pthread_mutex_unlock(&mutex);

    if(queue_operation_results == QUEUE_OPERATION_SUCESSFUL) { continue; }
    printf("Queue full!!!\n");
  }
}