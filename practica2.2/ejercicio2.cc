#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador : public Serializable
{
public:
    Jugador(const char* _n, int16_t _x, int16_t _y) :x(_x), y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador() {};

    void to_bin()
    {
        _size = (sizeof(int16_t) * 2) + sizeof(char) * 80;
        alloc_data(_size);

        char* aux = _data;
        memcpy(aux, name, sizeof(char) * 80); //Copiar el nombre
        aux += sizeof(char) * 80; //Movernos a siguiente dato
        memcpy(aux, &x, sizeof(int16_t)); //Copiar posicion x
        aux += sizeof(int16_t); //Movernos a siguiente dato
        memcpy(aux, &y, sizeof(int16_t)); //Copiar posicion y
    }

    int from_bin(char* data)
    {
        //
        return 0;
    }


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char** argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    // 1. Serializar el objeto one_w
    one_w.to_bin();

    // 2. Escribir la serialización en un fichero
    int archivo = open("player.txt", O_CREAT | O_TRUNC, 0666);

    write(archivo, one_w.data(), one_w.size());

    close(archivo);

    std::cout << one_w.name << " " << one_w.x << " " << one_w.y << std::endl;
    std::cout << "Archivo escrito" << std::endl;
    // 3. Leer el fichero
    // 4. "Deserializar" en one_r
    // 5. Mostrar el contenido de one_r

    return 0;
}

