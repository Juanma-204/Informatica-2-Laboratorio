#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "particula.h"

class Obstaculo
{
private:
    double x,y;
    double ancho, alto;
    double resistencia;
    double resistenciaInicial;

public:
    Obstaculo(double x, double y, double ancho, double alto, double resistencia_inicial);

    double getX() const;
    double getY() const;
    double getAncho() const;
    double getAlto() const;
    double getResistencia() const;
    void recibirDano(double dano);
    bool estaDestruido() const;

    double getResistenciaInicial() const;

    bool verificaColision(const Particula& p) const;

    int tipoDeRebote(const Particula& p) const;
};
#endif // OBSTACULO_H
