#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include "particula.h"
#include "walls.h"
#include "obstaculo.h"

class Simulador {

private:
    std::vector<Particula> particulas;
    std::vector<Obstaculo*> obstaculos;
    Walls paredes;

    int ancho;
    int alto;

public:
    Simulador(int n, int w, int h);

    void actualizar();
    void ejecutar();
    void guardarCSV(int tiempo);
};

#endif
