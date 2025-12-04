#ifndef PARTICULA_H
#define PARTICULA_H

class Particula {
public:
    float x, y;
    float vx, vy;
    float radio;
    float gravedad;

    Particula(float px = 0.0f, float py = 0.0f);

    void mover(); // mueve con gravedad (un paso por frame)
};

#endif // PARTICULA_H
