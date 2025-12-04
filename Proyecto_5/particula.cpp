#include "particula.h"

Particula::Particula(float px, float py)
{
    x = px;
    y = py;
    vx = 2.0f;     // velocidad horizontal inicial (puedes cambiar)
    vy = -10.0f;   // velocidad vertical inicial (negativa = hacia arriba)
    radio = 10.0f;
    gravedad = 0.4f; // ajuste de la gravedad por frame
}

void Particula::mover()
{
    // Aplicar gravedad (simple por frame)
    vy += gravedad;

    // Integración simple (por frame)
    x += vx;
    y += vy;
}
