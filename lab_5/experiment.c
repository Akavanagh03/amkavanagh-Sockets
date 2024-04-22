#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
    for(int i = 1; i < argc; i++){
        struct addrinfo hints;
        struct addrinfo *res;//pointer to the beginning of the link list
        struct addrinfo *p; //pointer to obejct in link list currently in use
        int status;
        char ipAddress[INET6_ADDRSTRLEN];

        if (argc < 2) { //make sure you have a domain name to go to
            fprintf(stderr,"usage: <executable name> hostname\n");
            return 1;
        }

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
        hints.ai_socktype = SOCK_STREAM;

        if ((status = getaddrinfo(argv[i], NULL, &hints, &res)) != 0) 
        {//attempt to connect to said domain and get info
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 2;
        }

        printf("IP addresses for %s:\n\n", argv[1]);

        for(p = res;p != NULL; p = p->ai_next) 
        //each object in the linked list holds the data for one ip address
        {//so we go through the linked list from the beginning and print out the info from each link
            void *addr;
            char *ipver;

            // get the pointer to the address itself,
            // different fields in IPv4 and IPv6:
            if (p->ai_family == AF_INET) 
            { // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            } 
            else 
            { // IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }

            // convert the IP to a string and print it:
            inet_ntop(p->ai_family, addr, ipAddress, sizeof ipAddress);
            printf("  %s: %s\n", ipver, ipAddress);
        }
        printf("\n");
        freeaddrinfo(res); // free the linked list
    }
    return 0;
}