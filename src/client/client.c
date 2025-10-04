#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define REQUEST_LENGTH 5
#define DATA_PACKET_LENGTH_IN_BYTES 40
#define handle_error(message) \
  do { perror(message); exit(EXIT_FAILURE); } while(0)

#define DATA_NONEXISTENT_ERROR -51

#define GET_REQUEST "GET"
#define STOP_REQUEST "STOP"

int main() {
  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_fd == -1) { handle_error("socket"); }

  const struct sockaddr_un server_address = {
    .sun_family = AF_UNIX,
    .sun_path = "/tmp/sensor_socket"
  };

  if(connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    handle_error("connect");
  }

  char request_buffer[REQUEST_LENGTH];

  strncpy(request_buffer, GET_REQUEST, 4);
  write(server_fd, request_buffer, REQUEST_LENGTH);

  double data_packet_buffer[DATA_PACKET_LENGTH_IN_BYTES / 8];
  int server_result_status = read(server_fd, data_packet_buffer, DATA_PACKET_LENGTH_IN_BYTES);
  
  if (server_result_status > 0) {
    printf("Temperature Data: {");
    for(int i = 0; i < (DATA_PACKET_LENGTH_IN_BYTES / 8) - 1; i++) {
      printf("%lf, ", data_packet_buffer[i]);
    }
    printf("%lf}\n", data_packet_buffer[(DATA_PACKET_LENGTH_IN_BYTES / 8) - 1]);
  }

  write(server_fd, STOP_REQUEST, strlen(STOP_REQUEST));
  exit(EXIT_SUCCESS); 
  return 0;
}