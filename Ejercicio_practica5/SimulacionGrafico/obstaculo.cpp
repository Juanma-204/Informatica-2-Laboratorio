#include "obstaculo.h"
#include "particula.h"
#include <cmath>
#include <algorithm>

Obstaculo::Obstaculo(double x, double y, double ancho, double alto, double resistencia_inicial)
    : x(x), y(y), ancho(ancho), alto(alto),
    resistencia(resistencia_inicial), resistenciaInicial(resistencia_inicial)
{

}

double Obstaculo::getX() const { return x; }
double Obstaculo::getY() const { return y; }
double Obstaculo::getAncho() const { return ancho; }
double Obstaculo::getAlto() const { return alto; }
double Obstaculo::getResistencia() const { return resistencia; }
double Obstaculo::getResistenciaInicial() const { return resistenciaInicial; }

void Obstaculo::recibirDano(double dano)
{
    this->resistencia -= dano;
    if (this->resistencia < 0) {
        this->resistencia = 0;
    }
}

bool Obstaculo::estaDestruido() const
{
    return resistencia <= 0;
}

bool Obstaculo::verificaColision(const Particula& p) const
{
    double p_x = p.getX();
    double p_y = p.getY();
    double p_r = p.getRadio();

    double rect_left = x;
    double rect_right = x + ancho;
    double rect_bottom = y;
    double rect_top = y + alto;

    double closestX = std::max(rect_left, std::min(p_x, rect_right));
    double closestY = std::max(rect_bottom, std::min(p_y, rect_top));

    double dist_x = p_x - closestX;
    double dist_y = p_y - closestY;

    return (dist_x * dist_x + dist_y * dist_y) < (p_r * p_r);
}

int Obstaculo::tipoDeRebote(const Particula& p) const
{
    if (!verificaColision(p)) return 0; // No hay colisión

    double centroObsX = this->x + this->ancho / 2.0;
    double centroObsY = this->y + this->alto / 2.0;

    double dx = p.getX() - centroObsX;
    double dy = p.getY() - centroObsY;

    double anchoCombinado = (this->ancho / 2.0) + p.getRadio();
    double altoCombinado = (this->alto / 2.0) + p.getRadio();

    double overlapX = anchoCombinado - std::abs(dx);
    double overlapY = altoCombinado - std::abs(dy);

    if (overlapX < overlapY) {
        return 1;
    } else {
        return 2;
    }
}
