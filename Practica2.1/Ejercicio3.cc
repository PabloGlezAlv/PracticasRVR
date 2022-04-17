#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char** argv) {

    struct addrinfo hints;
    struct addrinfo* res;
    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

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

    freeaddrinfo(res);


    char buffer[100];

    if (sendto(sd, argv[3], strlen(argv[3]) + 1, 0, res->ai_addr, res->ai_addrlen) == -1)
    {
        printf("Error: Fallo en el send \n");
        close(sd);
        return -1;
    }
    else if (recvfrom(sd, buffer, 99, 0, res->ai_addr, &res->ai_addrlen) == -1)
    {
        printf("Error: Fallo en el receive \n");
        close(sd);
        return -1;
    }


    printf("%s \n", buffer);

    close(sd);
    return 0;
}