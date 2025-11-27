#include "particula.h"
#include <cmath>
#include <QDebug>

Particula::Particula(double x_inicial, double y_inicial, double vx_inicial, double vy_inicial, double masa, double radio)
    : x(x_inicial), y(y_inicial), vx(vx_inicial), vy(vy_inicial), masa(masa), radio(radio)
{
    g = 9.8;
}


//Metodos principales para que funcionen el objeto o se mueva//

void Particula::mover(double delta_t)
{
    this->vy += g * delta_t;

    x += vx * delta_t;
    y += vy * delta_t;

    qDebug() << "Moviendo -> Y:" << y << " VY:" << vy << " Gravedad:" << g;
}

bool Particula::estaColisionando(Particula& otra)
{
    double dist_x = otra.getX() - this->x;
    double dist_y = otra.getY() - this->y;
    double dist_centros = std::sqrt(std::pow(dist_x, 2) + std::pow(dist_y, 2));

    double radios_sum = this->radio + otra.getRadio();

    return dist_centros < radios_sum;
}


//vuelve el apartado de los get//

double Particula::getX() const
{
    return x;
}

double Particula::getY() const
{
    return y;
}

double Particula::getVX() const
{
    return vx;
}

double Particula::getVY() const
{
    return vy;
}

double Particula::getMasa() const
{
    return masa;
}

double Particula::getRadio() const
{
    return radio;
}

//volvemos otra vez al apartado de los set, para modificar las variablñes//

void Particula::setVX(double nuevo_vx)
{
    this->vx = nuevo_vx;
}

void Particula::setVY(double nuevo_vy)
{
    this->vy = nuevo_vy;
}

void Particula::setMasa(double nueva_masa)
{
    this->masa = nueva_masa;
}

void Particula::setRadio(double nuevo_radio)
{
    this->radio = nuevo_radio;
}
void Particula::setG(double gravedad)
{
    this->g = gravedad;
}
