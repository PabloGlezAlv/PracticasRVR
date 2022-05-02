#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <stdlib.h>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Este interfaz debe implementarlo aquellos objetos que se transmitan por red
 *  Define una serializaci�n sencilla para la transmisi�n:
 *
 *  +--------+
 *  |        |  Esta es la representaci�n binaria del objeto. Cada objeto
 *  |        |  deber� implementar dos m�todos:
 *  |  data  |    - to_bin() para rellenar
 *  |        |    - from_bin() para reconstruirse a partir de data.
 *  |        |    - size es el tama�o total de la regi�n data
 *  +--------+
 */
class Serializable
{
public:

    Serializable() :_size(0), _data(0) {};

    virtual ~Serializable()
    {
        if (_data != 0)
        {
            free(_data);
        }
    }

    /**
     *  Genera la representaci�n binaria de la clase. Debe inicializar
     *  el buffer interno con la funci�n helper alloc_data.
     */
    virtual void to_bin() = 0;

    /**
     *  Esta funci�n recibe un objeto serializado y lo reconstruye.
     *    @param data representaci�n binaria del objeto
     *    @return 0 si �xito -1 en caso contrario
     */
    virtual int from_bin(char* data) = 0;

    /**
     *  Devuelve un puntero al buffer interno con la representaci�n del objeto.
     *  Debe inicializarse previamente via Serializable::to_bin()
     *    @return objeto serializado
     */
    char* data()
    {
        return _data;
    }

    /**
     *  @return tama�o del objeto serializado
     */
    int32_t size()
    {
        return _size;
    }

protected:

    int32_t _size;

    char* _data;

    /**
     *  Reserva memoria para el buffer del objeto serializado
     */
    void alloc_data(int32_t data_size)
    {
        if (_data != 0)
        {
            free(_data);
        }

        _data = (char*)malloc(data_size);
        _size = data_size;
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#endif /* SERIALIZABLE_H_ */
