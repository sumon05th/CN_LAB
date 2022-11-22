#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int factorial(int n)
{
    int res = 1;
    for (int i = 1; i <= n; i++)
        res *= i;
    return res;
}

int main()
{

    char buffer[1024] = {0};

    const int s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (s_fd < 0)
    {
        perror("Socket error\n");
        exit(1);
    }

    struct sockaddr_in s_addr, c_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(9000);
    s_addr.sin_addr.s_addr = INADDR_ANY;

    const int b_val = bind(s_fd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if (b_val < 0)
    {
        perror("Bind Error\n");
        exit(1);
    }

    int len = sizeof(c_addr);
    int n = recvfrom(s_fd, (char *)buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)&c_addr, &len);

    int query;
    sscanf(buffer, "%d", &query);
    // printf("%d", query);

    query = factorial(query);

    printf("Client: %s\n", buffer);

    sprintf(buffer, "%d", query);

    buffer[n] = '\0';

    sendto(s_fd, (const char *)buffer, sizeof(buffer), MSG_CONFIRM, (const struct sockaddr *)&c_addr, len);

    close(s_fd);

return 0;
}