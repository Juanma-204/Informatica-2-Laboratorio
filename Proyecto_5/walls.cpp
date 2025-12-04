#include "walls.h"

Walls::Walls(int w, int h) : width(w), height(h) { }

void Walls::colisionar(Particula &p) const {
    // Izquierda
    if (p.x - p.radio < 0.0f) {
        p.x = p.radio;
        p.vx = -p.vx;
    }
    // Derecha
    if (p.x + p.radio > width) {
        p.x = width - p.radio;
        p.vx = -p.vx;
    }
    // Superior
    if (p.y - p.radio < 0.0f) {
        p.y = p.radio;
        p.vy = -p.vy;
    }
    // Inferior
    if (p.y + p.radio > height) {
        p.y = height - p.radio;
        p.vy = -p.vy;
    }
}
