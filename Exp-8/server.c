#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "helper.h"

int main() {
  int socket_fd, connection_fd;

  char buff[MAX_SIZE];

  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket connection failed.\n");
    exit(1);
  };

  if ((bind(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address))) < 0) {
    perror("Socket bind failed.\n");
    exit(1);
  }

  if ((listen(socket_fd, 5)) < 0) {
    perror("Socket listening failed.\n");
    exit(1);
  }

  printf("Server listening to port %d.\n", PORT);

  while (1) {
    if ((connection_fd = accept(socket_fd, NULL, NULL)) < 0) {
      perror("Server accept failed.\n");
      exit(1);
    }

    read(connection_fd, buff, sizeof(buff));

    char *method, *url;
    get_method_and_url(buff, &method, &url);

    printf("URL: %s\n", url);
    printf("Method: %s\n", method);

    FILE* file;

    check_file_and_return(url, &file);

    send_response(connection_fd, file);

    close(connection_fd);
  }

  close(socket_fd);

  return 0;
}