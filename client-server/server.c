#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void filemode(int val,char buf[BUFFER_SIZE],int socket);
void echomode(int val, char buf[BUFFER_SIZE],int socket);


int main() 
{
//start socket setup
    int server_socket, client_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 3) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) 
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

//end of socket set up

    int option;
    valread = read(client_socket, &option, sizeof(option));

    if (option == 1) 
    {
        // Echo mode
        echomode(valread,buffer,client_socket);
    } 
    else if (option == 2) 
    {
        // File mode
        filemode(valread,buffer,client_socket);
    }

    close(client_socket);
    close(server_socket);

    return 0;
}

void filemode(int val,char buf[BUFFER_SIZE],int socket)
{
    FILE *fp = fopen("sendfile.txt", "r");
        if (fp == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        while ((val = fread(buf, 1, BUFFER_SIZE, fp)) > 0) {
            send(socket, buf, val, 0);
        }

        fclose(fp);
}
void echomode(int val, char buf[BUFFER_SIZE],int socket)
{
    while (1) {
            val = read(socket, buf, BUFFER_SIZE);
            if (val <= 0) {
                break;
            }
            printf("Client: %s", buf);
            send(socket, buf, val, 0);
            memset(buf, 0, BUFFER_SIZE);
        }
}