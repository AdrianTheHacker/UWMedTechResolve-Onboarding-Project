#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define handle_error(message) \
  do { perror(message); exit(EXIT_FAILURE); } while(0)

int main() {
  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(server_fd == -1) { handle_error("socket"); }

  const struct sockaddr_un server_address = {
    .sun_family = AF_UNIX,
    .sun_path = "/tmp/sensor_socket"
  };

  if(connect(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    handle_error("connect");
  }

  // Send a message
  const char *get_request = "GET";
  const char *stop_request = "STOP";
  double buffer[5];

  write(server_fd, get_request, 3);

  if (read(server_fd, buffer, sizeof(buffer)) > 0) {
    printf("Temperature Data: ");
    for(int i = 0; i < 5; i++) {
      printf("%f, ", buffer[i]);
    }
    printf("\n");
  }

  write(server_fd, stop_request, 4);
  
  exit(EXIT_SUCCESS);
  return 0;
}