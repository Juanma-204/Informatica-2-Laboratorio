#ifndef BALL_H
#define BALL_H

#include <string>

class Ball {
public:
    double x, y;
    double vx, vy;

    Ball(double x, double y, double vx, double vy);

    void mover();
    void rebotar(double width, double height);

    std::string toString() const;
};

#endif
