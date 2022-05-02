#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#include <iostream>
#include <stdexcept>

#include <ostream>

// -----------------------------------------------------------------------------
// Definiciones adelantadas
// -----------------------------------------------------------------------------
class Socket;
class Serializable;

/**
 *  Esta funci�n compara dos Socks, realizando la comparaci�n de las structuras
 *  sockaddr: familia (INET), direcci�n y puerto, ver ip(7) para comparar
 *  estructuras sockaddr_in. Deben comparar el tipo (sin_family), direcci�n
 *  (sin_addr.s_addr) y puerto (sin_port). La comparaci�n de los campos puede
 *  realizarse con el operador == de los tipos b�sicos asociados.
 */
bool operator== (const Socket& s1, const Socket& s2);

/**
 *  Imprime la direcci�n y puerto en n�mero con el formato:"direcci�n_ip:puerto"
 */
std::ostream& operator<<(std::ostream& os, const Socket& dt);
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


/**
 * Clase base que representa el extremo local de una conexi�n UDP. Tiene la l�gica
 * para inicializar un sockect y la descripci�n binaria del extremo
 *   - direcci�n IP
 *   - puerto
 */
class Socket
{
public:
    /**
     * El m�ximo te�rico de un mensaje UDP es 2^16, del que hay que
     * descontar la cabecera UDP e IP (con las posibles opciones). Se debe
     * utilizar esta constante para definir buffers de recepci�n.
     */
    static const int32_t MAX_MESSAGE_SIZE = 32768;

    /**
     *  Construye el socket UDP con la direcci�n y puerto dados. Esta funci�n
     *  usara getaddrinfo para obtener la representaci�n binaria de direcci�n y
     *  puerto.
     *
     *  Adem�s abrir� el canal de comunicaci�n con la llamada socket(2).
     *
     *    @param address cadena que representa la direcci�n o nombre
     *    @param port cadena que representa el puerto o nombre del servicio
     */
    Socket(const char* address, const char* port);

    /**
     *  Inicializa un Socket copiando los par�metros del socket
     */
    Socket(struct sockaddr* _sa, socklen_t _sa_len) :sd(-1), sa(*_sa),
        sa_len(_sa_len) {};

    virtual ~Socket() {};

    /**
     *  Recibe un mensaje de aplicaci�n
     *
     *    @param obj que recibir� los datos de la red. Se usar� para la
     *    reconstrucci�n del objeto mediante Serializable::from_bin del interfaz.
     *
     *    @param sock que identificar� al extremo que env�a los datos si es
     *    distinto de 0 se crear� un objeto Socket con la direcci�n y puerto.
     *
     *    @return 0 en caso de �xito o -1 si error (cerrar conexi�n)
     */
    int recv(Serializable& obj, Socket*& sock);

    int recv(Serializable& obj) //Descarta los datos del otro extremo
    {
        Socket* s = 0;

        return recv(obj, s);
    }

    /**
     *  Env�a un mensaje de aplicaci�n definido por un objeto Serializable.
     *
     *    @param obj en el que se enviar� por la red. La funci�n lo serializar�
     *
     *    @param sock con la direcci�n y puerto destino
     *
     *    @return 0 en caso de �xito o -1 si error
     */
    int send(Serializable& obj, const Socket& sock);

    /**
     *  Enlaza el descriptor del socket a la direcci�n y puerto
     */
    int bind()
    {
        return ::bind(sd, (const struct sockaddr*)&sa, sa_len);
    }

    friend std::ostream& operator<<(std::ostream& os, const Socket& dt);

    friend bool operator== (const Socket& s1, const Socket& s2);

    friend bool operator!= (const Socket& s1, const Socket& s2);

protected:

    /**
     *  Descriptor del socket
     */
    int sd;

    /**
     *  Representaci�n binaria del extremo, usada por servidor y cliente
     */
    struct sockaddr sa;
    socklen_t       sa_len;
};

#endif /* SOCKET_H_ */
