#include "simulador.h"
#include <iostream>
#include <algorithm>

Obstaculo* Simulador::getObstaculo(int index) const {
    if (index >= 0 && index < obstaculos.size()) {
        return obstaculos[index];
    }
    return nullptr;
}

Simulador::Simulador(double ancho, double alto, double e)
    : anchoCaja(ancho), altoCaja(alto), coef_restitucion(e)
{
}

Simulador::~Simulador() {
    for (Particula* p : particulas) {
        delete p;
    }
    for (Obstaculo* o : obstaculos) {
        delete o;
    }
}

void Simulador::agregarParticula(Particula* p) {
    particulas.push_back(p);
}

void Simulador::agregarObstaculo(Obstaculo* o) {
    obstaculos.push_back(o);
}

//Este seria como el apartado de las paredes, o lo que delimita al objeto o particula de que no siga avanzando y rebote//
void Simulador::resolverColisionesParedes(Particula* p)
{

    if (p->getX() + p->getRadio() >= anchoCaja) {
        p->setVX( -std::abs(p->getVX()) );
    }

    else if (p->getX() - p->getRadio() <= 0) {
        p->setVX( std::abs(p->getVX()) );
    }

    if (p->getY() + p->getRadio() >= altoCaja) {
        p->setVY( -std::abs(p->getVY()) );
    }

    else if (p->getY() - p->getRadio() <= 0) {
        p->setVY( std::abs(p->getVY()) );
    }
}

//Lo mismo, solo que de manera inelastica, y con un obstaculo u objeto como le quiera decir//
void Simulador::resolverColisionesObstaculos(Particula* p)
{
    for (Obstaculo* obs : obstaculos) {
        if (!obs->estaDestruido() && obs->verificaColision(*p)) {

            double dano = p->getMasa();
            obs->recibirDano(dano);

            p->setVX(0.0);
            p->setVY(0.0);

            return;
        }
    }
}

void Simulador::resolverColisionesParticulas()
{

    for (auto it1 = particulas.begin(); it1 != particulas.end(); ++it1) {
        Particula* p1 = *it1;

        for (auto it2 = std::next(it1); it2 != particulas.end();) {
            Particula* p2 = *it2;

            if (p1->estaColisionando(*p2)) {

                double m1 = p1->getMasa();
                double m2 = p2->getMasa();
                double M = m1 + m2;

                double vNuevaX = (m1 * p1->getVX() + m2 * p2->getVX()) / M;
                double vNuevaY = (m1 * p1->getVY() + m2 * p2->getVY()) / M;

                p1->setVX(vNuevaX);
                p1->setVY(vNuevaY);

                p1->setMasa(M);

                double areaTotal = std::pow(p1->getRadio(), 2) + std::pow(p2->getRadio(), 2);
                p1->setRadio(std::sqrt(areaTotal));

                delete p2;

                it2 = particulas.erase(it2);
            } else {
                ++it2;
            }
        }
    }
}

void Simulador::pasoDeSimulacion(double dt)
{
    for (Particula* p : particulas) {
        p->mover(dt);
    }

    resolverColisionesParticulas();

    for (Particula* p : particulas) {
        resolverColisionesParedes(p);
        resolverColisionesObstaculos(p);
    }
}

//Despues se guarda en el estado en el que quedo//
void Simulador::guardarEstado(std::ofstream& archivo, double tiempoActual)
{
    archivo << tiempoActual;
    for (const Particula* p : particulas) {
        archivo << "\t" << p->getX() << "\t" << p->getY();
    }
    archivo << std::endl;
}

void Simulador::eliminarParticula(Particula* p) {
    auto it = std::find(particulas.begin(), particulas.end(), p);

    if (it != particulas.end()) {
        delete *it;
        particulas.erase(it);
    }
}

int Simulador::getNumObstaculos() const
{
    return obstaculos.size();
}




