#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

int main()
{

    char buffer[1024] = {0};
    char hello[1024];

    int s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (s_fd < 0)
    {
        perror("Socket failed\n");
        exit(1);
    }

    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(9000);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    scanf("%[^\n]", hello);



    sendto(s_fd, (const char *)hello, sizeof(hello), MSG_CONFIRM, (const struct sockaddr *)&s_addr, sizeof(s_addr));

    printf("Message sent\n");

    int len;
    int n = recvfrom(s_fd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&s_addr, &len);



    printf("Server: %s\n", buffer);



    close(s_fd);
return 0;
}