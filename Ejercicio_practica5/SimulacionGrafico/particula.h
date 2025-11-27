#ifndef PARTICULA_H
#define PARTICULA_H

#include <math.h>

class Particula
{
private:
    double x, y;
    double vx, vy;
    double masa;
    double radio;
    double g;

public:
    Particula(double x_inicial, double y_inicial, double vx_inicial, double vy_inicial, double masa, double radio);

    //metodos principales//
    void mover(double delta_t);
    bool estaColisionando(Particula& otra);

    //Los metodos get es para obtener los valores//
    double getX() const;
    double getY() const;
    double getVX() const;
    double getVY() const;
    double getMasa() const;
    double getRadio() const;

    //por otro lado los metodos set, son como lo dice la palabra al español, para poner, para establecer o modificar un parametro//
    void setVX(double nuevo_vx);
    void setVY(double nuevo_vy);
    void setMasa(double nueva_masa);
    void setRadio(double nuevo_radio);
    void setG(double gravedad);

};

#endif // PARTICULA_H
