#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void echomode(int sock,char buffer[BUFFER_SIZE],int val);
void filemode(int val, char buf[BUFFER_SIZE], int sock);

int main() 
{
// socket setup start
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    int option;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        perror("Socket creation error");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
    {
        perror("Invalid address/ Address not supported");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        perror("Connection Failed");
        return -1;
    }
// socket setup end

    printf("Enter 1 for echo mode, 2 for file mode: ");
    scanf("%d", &option);
    send(sock, &option, sizeof(option), 0);

    if (option == 1) 
    {
        // Echo mode
        echomode(sock,buffer,valread);

    }
    else if (option == 2) 
    {
        // File mode
        filemode(valread,buffer,sock);
    }

    close(sock);

    return 0;
}

void echomode(int sock,char buffer[BUFFER_SIZE],int val)
{
    while (1) 
        {
            printf("Enter message: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            send(sock, buffer, strlen(buffer), 0);
            if (strncmp(buffer, "quit", 4) == 0) {
                break;
            }
            memset(buffer, 0, BUFFER_SIZE);
            val = read(sock, buffer, BUFFER_SIZE);
            printf("Server: %s", buffer);
        }
}
void filemode(int val, char buf[BUFFER_SIZE], int sock)
{
    FILE *fp = fopen("received_file.txt", "w");
    if (fp == NULL) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while ((val = read(sock, buf, BUFFER_SIZE)) > 0) 
    {
        fwrite(buf, 1, val, fp);
    }

        fclose(fp);
        printf("File received successfully.\n");
}