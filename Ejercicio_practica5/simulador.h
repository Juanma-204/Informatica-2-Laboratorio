#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <vector>
#include <fstream>
#include "particula.h"
#include "obstaculo.h"

class Simulador
{
private:
//Estos vendrian siendo como las paredes de las que se hablaron en clase//
    double anchoCaja;
    double altoCaja;

    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;

    //Para choques con los obstaculos
    double coef_restitucion;

    //Este seria un choque o una colision elastica o perfectamente elastica//
    void resolverColisionesParedes(Particula& p);

    //Este seria un choque o una colision inelastica//
    void resolverColisionesObstaculos(Particula& p);

    //Este seria el choque o colision entre particulas, perfectamente inelastico//
    void resolverColisionesParticulas();

public:
    //Tamaño de las cajas (paredes)//
    Simulador(double ancho, double alto, double e);

    void agregarParticula(Particula p);
    void agregarObstaculo(Obstaculo o);

    void pasoDeSimulacion(double dt);

    void guardarEstado(std::ofstream& archivo, double tiempoActual);
};

#endif // SIMULADOR_H
