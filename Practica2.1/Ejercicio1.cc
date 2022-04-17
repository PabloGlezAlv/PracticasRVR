#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Numero de argumentos invalido\n");
        return -1;
    }

    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE; //Devuelve 0.0.0.0 
    hints.ai_family = AF_UNSPEC; //Cualquier familia

    int sol = getaddrinfo(argv[1], argv[2], &hints, &res);

    if (sol != 0) //Si da error
    {
        printf("Error: %s\n", gai_strerror(sol));
    }
    else
    {
        struct addrinfo* i;

        for (i = res; i != NULL; i = i->ai_next)
        {
            char host[NI_MAXHOST];
            char server[NI_MAXSERV];

            getnameinfo(i->ai_addr, i->ai_addrlen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST);

            printf("%s %d %d \n", host, res->ai_family, res->ai_socktype);
        }
    }

    freeaddrinfo(res);

    return 0;
}