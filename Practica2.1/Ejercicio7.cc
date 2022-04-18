#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#define MAXLISTEN 3

const size_t MAXBUFFER = 80;
int numListen = 0;

std::condition_variable numCv;
std::mutex numMutex;

class MyThread {
private:
	int client;
	int id;
public:
	MyThread(int cliente, int i) :
	{
		client = cliente;
		id = i;
	};

	void connections() {
		bool activo = true;

		while (activo) {
			char buffer[MAXBUFFER];

			int bytes = recv(clientesd_, (void*)buffer, MAXBUFFER - 1, '\0');
			if (bytes < 1)
			{
				std::cout << "Conexión terminada\n";
				activo = false;
			}
			buffer[bytes] = '\0';

			send(clientesd_, (void*)buffer, bytes, 0);
		}

		close(clientesd_);

		numMutex.lock();
		numListen--;

		if (numListen < MAXLISTEN) {
			numCv.notify_all();
		}
		numMutex.unlock();
	}
};

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Uso: ./Ejercicio7 [IP] [puerto]\n";
		return -1;
	}

	struct addrinfo hints;
	struct addrinfo* res;

	memset((void*)&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (sol != 0)
	{
		printf("Error: %s\n", gai_strerror(sol));
		return -1;
	}
	else if (sd == -1) {
		printf("Error: Socket mal creado\n");
		return -1;
	}

	if (bind(sd, res->ai_addr, res->ai_addrlen) == -1) {
		printf("Error: Fallo de la direccion al socket\n");
		return -1;
	}

	freeaddrinfo(res);

	if (listen(sd, MAXLISTEN) == -1) {
		printf("Error: Fallo en listen\n");
		return -1;
	}

	while (true) {
		struct sockaddr cliente;
		socklen_t clientelen = sizeof(struct sockaddr);

		{
			std::unique_lock<std::mutex> lck(numMutex);
			while (numListen >= MAXLISTEN) {
				printf("Servidor lleno\n");
				numCv.wait(lck);
			}
		}

		int clientSd = accept(sd, (struct sockaddr*)&cliente, &clientelen);
		if (clientSd == -1)
		{
			printf("ERROR: fallo en accept\n");
			close(sd);
			return -1;
		}

		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];
		getnameinfo((struct sockaddr*)&cliente, clientelen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);

		printf("Conexion desde %s %s\n", host, server);

		numMutex.lock();
		numListen++;
		numMutex.unlock();

		MyThread* thrd = new MyThread(clientesd, numListen);
		std::thread([&thrd]()
			{
				thrd->connections();
				delete thrd;
			}).detach();
	}

	close(sd);
	return 0;
}