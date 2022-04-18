#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <thread>
#include <iostream>

class Message {
private:
	int sd;
	int id;
public:
	Message(int s, int i)
	{
		sd = s;
		id = i;
	}
	void creaMensaje() {

		bool bucle = true;
		char buffer[100];
		char host[NI_MAXHOST];
		char server[NI_MAXSERV];

		struct sockaddr client;
		socklen_t clientLen = sizeof(struct sockaddr);

		while (bucle)
		{
			int data = recvfrom(sd, (void*)buffer, 99, '\0', &client, &clientLen);

			if (data == -1)
			{
				printf("ERROR: no se reciben bytes\n");
				return;
			}

			buffer[data] = '\0';

			getnameinfo(&client, clientLen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

			printf("Tenemos %d bytes del %s : %s", data, host, server);

			time_t tiempo;
			size_t size;

			switch (buffer[0]) {
			case 't': //LA HORA
				time(&tiempo);

				size = strftime(buffer, 99, "%p", localtime(&tiempo));
				sendto(sd, buffer, size, 0, &client, clientLen);

				buffer[size] = '\0';
				break;
			case 'd': //LA FECHA
				time(&tiempo);

				size = strftime(buffer, 99, "%F", localtime(&tiempo));
				sendto(sd, buffer, size, 0, &client, clientLen);

				buffer[size] = '\0';
				break;
			default:
				printf("Comando no soportado X");
				break;
			}
		}
	}
};

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

	for (int i = 0; i < 4; i++) { //10 MAX NUM DE HILOS
		Message* thr = new Message(sd, i);
		std::thread([&thr]()
			{ thr->creaMensaje();
		delete thr;
			}).detach();
	}

	char q = 'a';
	while (q != 'q') {
		scanf("%c", &q);
	}
	printf("Saliendo...");
	close(sd);
	return 0;
}