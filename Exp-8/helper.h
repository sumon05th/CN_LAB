#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_SIZE 10000
#define R_LIM 100
#define PORT 5000

/// @brief Get's the HTTP method name and requested URL from HTTP headers
/// @param header The HTTP request header
/// @param method The method name extacted
/// @param url The URL extracted
void get_method_and_url(char* header, char** method, char** url) {
  int size_of_method, size_of_url;

  // Get the first line of HTTP Request Header
  char* request = strtok(header, "\n");

  // Get the first word (Method name) of the first line of HTTP Header
  char* split_request = strtok(request, " ");
  *method = split_request;

  // Get the second word (URL) of the first line of HTTP Header
  split_request = strtok(NULL, " ");
  *url = split_request;
}

/// @brief Checks the filename of the requested resource if exist then sends
/// @param filename The filename requested. if exists, sends the file else sends error.html
/// @param file The file pointer of the requested file can be accessed
void check_file_and_return(const char* filename, FILE** file) {
  // If accessing `/` we send index.html
  if (strlen(filename) == 1 && filename[0] == '/') {
    filename = "/index.html";
  }

  // Appending the filename with `public`
  char file_path[MAX_SIZE] = "./public";
  strcat(file_path, filename);
  // If file exists, then send the file, else send `error.html`
  if (access(file_path, F_OK) == 0) {
    *file = fopen(file_path, "r");
  } else {
    *file = fopen("./public/error.html", "r");
  }
}

/// @brief Sends the response for the requested file with proper HTTP response headers and appending the requested file
/// @param connection_fd The connection file descriptor of the web client
/// @param file The file to be sent
void send_response(int connection_fd, FILE* file) {
  int file_content_size;
  // Need to append the HTML Content after this header
  char http_header[MAX_SIZE] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";

  char line[R_LIM];
  char response_data[MAX_SIZE] = {}, content_size[MAX_SIZE];

  while (fgets(line, R_LIM, file) != 0) {
    strcat(response_data, line);
  }

  file_content_size = strlen(response_data) * sizeof(char);
  sprintf(content_size, "%d\r\n\n", file_content_size);

  strcat(http_header, content_size);

  strcat(http_header, response_data);

  send(connection_fd, http_header, sizeof(http_header), 0);
}