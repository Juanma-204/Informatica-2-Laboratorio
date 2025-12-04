#include "obstaculo.h"
#include <cmath>

//
// ---------- CUADRADO ----------
//
ObstaculoCuadrado::ObstaculoCuadrado(float x_, float y_, float a, float h)
    : x(x_), y(y_), ancho(a), alto(h) {}

bool ObstaculoCuadrado::colisionar(Particula &p) {

    // choque horizontal
    if (p.y > y && p.y < y + alto) {
        if (p.x + p.radio > x && p.x < x + ancho) {
            p.vx *= -1;
            return true;
        }
    }

    // choque vertical
    if (p.x > x && p.x < x + ancho) {
        if (p.y + p.radio > y && p.y < y + alto) {
            p.vy *= -1;
            return true;
        }
    }

    return false;
}

//
// ---------- TRIÁNGULO ----------
//

ObstaculoTriangulo::ObstaculoTriangulo(Punto a, Punto b, Punto c)
    : A(a), B(b), C(c) {}

static float area(float x1,float y1,float x2,float y2,float x3,float y3){
    return std::abs((x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2))/2.0f);
}

bool ObstaculoTriangulo::puntoDentroTriangulo(float px, float py) {
    float A0 = area(A.x, A.y, B.x, B.y, C.x, C.y);
    float A1 = area(px, py, B.x, B.y, C.x, C.y);
    float A2 = area(A.x, A.y, px, py, C.x, C.y);
    float A3 = area(A.x, A.y, B.x, B.y, px, py);

    return std::abs((A1 + A2 + A3) - A0) < 0.1f;
}

bool ObstaculoTriangulo::colisionar(Particula &p) {
    if (puntoDentroTriangulo(p.x, p.y)) {
        p.vx *= -1;
        p.vy *= -1;
        return true;
    }
    return false;
}
