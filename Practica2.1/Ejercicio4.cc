#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv) {
    if (argc != 3)
    {
        printf("Numero de argumentos invalido\n");
        return -1;

    }

    struct addrinfo hints;
    struct addrinfo* res;
    memset((void*)&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int sol = getaddrinfo(argv[1], argv[2], &hints, &res);
    int sd = socket(res->ai_family, res->ai_socktype, 0);

    if (sol != 0) //Si da error
    {
        printf("Error: %s\n", gai_strerror(sol));
        return -1;
    }
    else if (sd == -1)
    {
        printf("Error: Socket mal creado\n");
        return -1;
    }

    if (bind(sd, res->ai_addr, res->ai_addrlen) == -1) {
        printf("Error: Fallo de la direccion al socket\n");
        return -1;
    }

    freeaddrinfo(res);

    bool bucle = true;
    char buffer[100];
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];

    if (listen(sd, 1) == -1) {
        printf("Error: Fallo en listen\n");
        return -1;
    }

    struct sockaddr client;
    socklen_t clientLen = sizeof(struct sockaddr);
    int clientSd = accept(sd, (struct sockaddr*)&client, &clientLen);

    if (clientSd == -1) {
        printf("Error: Fallo en accept\n");
        close(clientSd);
        close(sd);
        return -1;
    }

    getnameinfo(&client, clientLen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);


    printf("Conexion desde %s %s\n", host, server);

    while (bucle)
    {
        int data = recvfrom(clientSd, (void*)buffer, 99, '\0', &client, &clientLen);
        if (data <= 0)
        {
            printf("Conexion terminada\n");
            bucle = false;
        }
        else
        {
            buffer[data] = '\0';
            sendto(clientSd, buffer, data, 0, &client, clientLen);
        }
    }
    close(clientSd);
    close(sd);
    return 0;
}






