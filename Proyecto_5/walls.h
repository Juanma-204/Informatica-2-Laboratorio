#ifndef WALLS_H
#define WALLS_H

#include "particula.h"

class Walls {
private:
    int width;
    int height;

public:
    Walls(int w = 800, int h = 600);

    // Ajusta posición/velocidad si choca con las paredes
    void colisionar(Particula &p) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif // WALLS_H
