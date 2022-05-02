#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* aux = _data;
    memcpy(aux, &type, sizeof(uint8_t));
    aux += sizeof(uint8_t);
    memcpy(aux, nick.c_str(), sizeof(char) * 8); //Tamaño nombre
    aux += sizeof(char) * 8;
    memcpy(aux, message.c_str(), sizeof(char) * 80);
}

int ChatMessage::from_bin(char* bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void*>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char* aux = _data;
    memcpy(&type, aux, sizeof(uint8_t));
    aux += sizeof(uint8_t);
    nick = aux;
    aux += sizeof(char) * 8;
    message = aux;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

         //Recibir Mensajes en y en función del tipo de mensaje
         // - LOGIN: Añadir al vector clients
         // - LOGOUT: Eliminar del vector clients
         // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)

        ChatMessage m;
        Socket* s;
        socket.recv(m, s);

        if (m.type == ChatMessage::LOGIN)
        {
            std::cout << "Se ha conectado el jugador " << m.nick << std::endl;
            clients.push_back(std::move(std::make_unique<Socket>(*s)));
        }
        else if (m.type == ChatMessage::LOGOUT)
        {
            auto it = clients.begin();

            while (it != clients.end() && (**it != *s)) //Buscar el cliente que se desconecta
                ++it;

            if (it != clients.end())
            {
                std::cout << "Se ha desconectado el jugador " << m.nick << std::endl;
                clients.erase(it);
                Socket* delSock = (*it).release();
                delete delSock;
            }
        }
        else if (m.type == ChatMessage::MESSAGE)
        {
            for (auto it = clients.begin(); it != clients.end(); it++)
            {
                if (**it != *s)    socket.send(m, **it);
            }
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    // Completar

    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        // Leer stdin con std::getline
        std::string msg;
        std::getline(std::cin, msg);

        // Enviar al servidor usando socket
        ChatMessage m(nick, msg);
        m.type = ChatMessage::MESSAGE;
        socket.send(m, socket);
    }
}

void ChatClient::net_thread()
{
    while (true)
    {
        //Recibir Mensajes de red
        ChatMessage m;
        socket.recv(m);

        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        std::cout << m.nick << ": " << m.message << std::endl;
    }
}