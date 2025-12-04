#include "simulador.h"
#include <fstream>
#include <cmath>
#include <iostream>
#include "walls.h"
#include "obstaculo.h"

Simulador::Simulador(int n, int w, int h)
    : paredes(w, h), ancho(w), alto(h)
{
    particulas.reserve(n);
    for (int i = 0; i < n; i++) {
        particulas.emplace_back(100 + i * 20, 300);
    }

    //
    // ======== AGREGAR OBSTÁCULOS ========
    //

    // Cuadrado: posición (300,200), tamaño 80x80
    obstaculos.push_back(new ObstaculoCuadrado(300, 200, 80, 80));

    // Triángulo con puntos (500,100), (550,200), (450,200)
    obstaculos.push_back(
        new ObstaculoTriangulo(
            {500, 100},
            {550, 200},
            {450, 200}
            )
        );

    //
    // ======== CREAR CSV ========
    //
    std::ofstream archivo("datos.csv");
    archivo << "tiempo,id,x,y\n";
    archivo.close();
}

void Simulador::actualizar() {

    // ======== MOVER Y REBOTAR CON PAREDES Y OBSTÁCULOS ========
    for (auto &p : particulas) {
        p.mover();              // gravedad + movimiento
        paredes.colisionar(p);  // rebote con paredes

        // rebote con obstáculos
        for (auto &obs : obstaculos) {
            obs->colisionar(p);
        }
    }

    // ======== COLISIÓN ENTRE PARTÍCULAS ========
    for (int i = 0; i < particulas.size(); i++) {
        for (int j = i + 1; j < particulas.size(); j++) {

            float dx = particulas[j].x - particulas[i].x;
            float dy = particulas[j].y - particulas[i].y;
            float dist = std::sqrt(dx*dx + dy*dy);

            if (dist < particulas[i].radio * 2) {
                std::swap(particulas[i].vx, particulas[j].vx);
                std::swap(particulas[i].vy, particulas[j].vy);
            }
        }
    }
}

void Simulador::guardarCSV(int tiempo) {
    std::ofstream archivo("datos.csv", std::ios::app);

    for (int i = 0; i < particulas.size(); i++) {
        archivo << tiempo << ","
                << i << ","
                << particulas[i].x << ","
                << particulas[i].y << "\n";
    }

    archivo.close();
}

void Simulador::ejecutar() {
    for (int t = 10; t < 150; t++) {
        actualizar();
        guardarCSV(t);

        // impresión en consola
        for (auto &p : particulas)
            std::cout << p.x << "," << p.y << "\n";

        std::cout << "------------------\n";
    }
}
