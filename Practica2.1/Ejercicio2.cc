#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

const size_t MAXBUFFER = 80;

int main(int argc, char** argv) {
	if (argc != 3)
	{
		printf("Numero de argumentos invalido\n");
		return -1;

	}

	struct addrinfo hints;
	struct addrinfo* res;

	memset((void*)&hints, 0, sizeof(struct addrinfo));

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

	if (bind(sd, res->ai_addr, res->ai_addrlen) == -1) {
		printf("Error: Fallo de la direccion al socket\n");
		return -1;
	}

	freeaddrinfo(res);

	bool bucle = true;
	char buffer[MAXBUFFER];
	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	while (bucle) {
		struct sockaddr client;
		socklen_t clientLen = sizeof(struct sockaddr);
		int bytes = recvfrom(sd, (void*)buffer, MAXBUFFER - 1, '\0', &client, &clientLen);
		if (bytes == -1) {
			printf("Error: No hay bytes");
			close(sd);
			return -1;
		}
		buffer[bytes] = '\0';

		getnameinfo(&client, clientLen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf("%d bytes de %s : %s", bytes, host, server);

		time_t tiempo;
		size_t size;

		switch (buffer[0]) {
		case 't': //LA HORA
			time(&tiempo);

			size = strftime(buffer, MAXBUFFER - 1, "%r", localtime(&tiempo));
			sendto(sd, buffer, size, 0, &client, clientLen);

			buffer[size] = '\0';
			break;
		case 'd': //LA FECHA
			time(&tiempo);

			size = strftime(buffer, MAXBUFFER - 1, "%F", localtime(&tiempo));
			sendto(sd, buffer, size, 0, &client, clientLen);

			buffer[size] = '\0';
			break;
		case 'q': //SALIR
			printf("Saliendo...");
			bucle = false;
			break;
		default:
			printf("Comando no soportado X");
			break;
		}
	}

	close(sd);
	return 0;
}