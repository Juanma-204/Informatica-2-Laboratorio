#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "particula.h"

class Obstaculo
{
private:
    double x;
    double y;
    double ancho;
    double alto;

public:
    Obstaculo(double x, double y, double ancho, double alto);

    double getX() const;
    double getY() const;
    double getAncho() const;
    double getAlto() const;

    bool verificaColision(const Particula& p) const;

    int tipoDeRebote(const Particula& p) const;
};
#endif // OBSTACULO_H
