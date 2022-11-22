#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int compare_strings(char a[], char b[])
{
    int c = 0;
    while (a[c] == b[c]) 
    {
        if (a[c] == '\0' || b[c] == '\0')
        break;
        c++;
    }
    if (a[c] == '\0' && b[c] == '\0')
    return 0;
    else
    return -1;
}

int main() {
    int welcomeSocket, newSocket, Client2;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    char buffer[1024];

    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7891);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.01");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    if (listen(welcomeSocket,5)==0)
        printf("Server is Listening\n");
    else
        printf("Error\n");

    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
    Client2 = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

    int cmdEXIT = 0;

    while (cmdEXIT == 0)
    {
        recv(newSocket, buffer, 1024, 0);
        printf ("%s\nMeesage from Client2\n", buffer);
        send(Client2,buffer,1024,0);
        if (compare_strings(buffer, "exit")==0)
        {   
            cmdEXIT = 1;
        }
        else 
        {
            memset(&buffer[0], 0, sizeof(buffer));  
            recv(Client2, buffer, 1024, 0);
            printf ("%s\n Message Client1\n", buffer);
            send(newSocket,buffer,1024,0);
            if (compare_strings(buffer, "exit")==0)
            {
                cmdEXIT = 1;
            }
        }
    }

    return 0;
}
