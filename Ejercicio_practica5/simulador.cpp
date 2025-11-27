#include "simulador.h"
#include <iostream>

Simulador::Simulador(double ancho, double alto, double e)
    : anchoCaja(ancho), altoCaja(alto), coef_restitucion(e)
{
}

void Simulador::agregarParticula(Particula p) {
    particulas.push_back(p);
}

void Simulador::agregarObstaculo(Obstaculo o) {
    obstaculos.push_back(o);
}

//Este seria como el apartado de las paredes, o lo que delimita al objeto o particula de que no siga avanzando y rebote//
void Simulador::resolverColisionesParedes(Particula& p)
{

    if (p.getX() + p.getRadio() >= anchoCaja) {
        p.setVX( -abs(p.getVX()) );
    }

    else if (p.getX() - p.getRadio() <= 0) {
        p.setVX( abs(p.getVX()) );
    }

    if (p.getY() + p.getRadio() >= altoCaja) {
        p.setVY( -abs(p.getVY()) );
    }

    else if (p.getY() - p.getRadio() <= 0) {
        p.setVY( abs(p.getVY()) );
    }
}

//Lo mismo, solo que de manera inelastica, y con un obstaculo u objeto como le quiera decir//
void Simulador::resolverColisionesObstaculos(Particula& p)
{
    for (const Obstaculo& obs : obstaculos) {
        int tipo = obs.tipoDeRebote(p);

        if (tipo == 1) {
            p.setVX( -p.getVX() * coef_restitucion );
        }
        else if (tipo == 2) {
            p.setVY( -p.getVY() * coef_restitucion );
        }
    }
}

void Simulador::resolverColisionesParticulas()
{
    // Choque entre partículas: Fusión (Inelástico perfecto) [cite: 85-90]
    // Nota: Esto es complejo porque al borrar elementos del vector mientras
    // lo recorres, puedes causar errores. Se debe hacer con cuidado.

    for (size_t i = 0; i < particulas.size(); ++i) {
        for (size_t j = i + 1; j < particulas.size(); ++j) {

            if (particulas[i].estaColisionando(particulas[j])) {
                Particula& p1 = particulas[i];
                Particula& p2 = particulas[j];

                double m1 = p1.getMasa();
                double m2 = p2.getMasa();
                double M = m1 + m2;

                double vNuevaX = (m1 * p1.getVX() + m2 * p2.getVX()) / M;
                double vNuevaY = (m1 * p1.getVY() + m2 * p2.getVY()) / M;

                p1.setVX(vNuevaX);
                p1.setVY(vNuevaY);

                particulas.erase(particulas.begin() + j);
                j--;
            }
        }
    }
}

void Simulador::pasoDeSimulacion(double dt)
{
    //Aca se mueven las parcitulas
    for (Particula& p : particulas) {
        p.mover(dt);
    }

//Verificamos y resolvemos las colisiones o choques//
    resolverColisionesParticulas();

    for (Particula& p : particulas) {
        resolverColisionesParedes(p);
        resolverColisionesObstaculos(p);
    }
}

//Despues se guarda en el estado en el que quedo//
void Simulador::guardarEstado(std::ofstream& archivo, double tiempoActual)
{
    archivo << tiempoActual;
    for (const Particula& p : particulas) {
        archivo << "\t" << p.getX() << "\t" << p.getY();
    }
    archivo << std::endl;
}
