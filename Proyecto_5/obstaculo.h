#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "particula.h"
#include <vector>

class Obstaculo {
public:
    virtual bool colisionar(Particula &p) = 0;
    virtual ~Obstaculo() {}
};

class ObstaculoCuadrado : public Obstaculo {
public:
    float x, y, ancho, alto;
    ObstaculoCuadrado(float x_, float y_, float a, float h);

    bool colisionar(Particula &p) override;
};

class ObstaculoTriangulo : public Obstaculo {
public:
    struct Punto { float x, y; };
    Punto A, B, C;

    ObstaculoTriangulo(Punto a, Punto b, Punto c);
    bool colisionar(Particula &p) override;

private:
    bool puntoDentroTriangulo(float px, float py);
};

#endif
