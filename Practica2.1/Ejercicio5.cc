
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

    int server = connect(sd, (struct sockaddr*)res->ai_addr, res->ai_addrlen);

    if (server == 1)
    {
        printf("Error: No se puede conectar con el server\n");
        close(sd);
        close(server);
        return -1;
    }
    else
    {
        freeaddrinfo(res);

        bool bucle = true;

        while (bucle)
        {
            char message[100];
            fgets(message, 100, stdin);

            size_t size = strlen(message);
            if (size == 2 && (message[0] == 'Q' || message[0] == 'q'))
            {
                bucle = false;
                break;
            }
            if (send(sd, message, size, 0) <= 0)
            {
                bucle = false;
                break;
            }

            char response[100];
            ssize_t received = recv(sd, response, sizeof(char) * (99), 0);

            if (received <= 0)
            {
                bucle = false;
                break;
            }

            printf("%.*s", static_cast<int>(received), response);
        }

        close(sd);

        return 0;
    }
}






