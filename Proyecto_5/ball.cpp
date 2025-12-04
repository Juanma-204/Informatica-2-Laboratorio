#include "ball.h"
#include <sstream>

Ball::Ball(double x, double y, double vx, double vy)
    : x(x), y(y), vx(vx), vy(vy) {}

void Ball::mover() {
    x += vx;
    y += vy;
}

void Ball::rebotar(double width, double height) {
    if (x <= 0 || x >= width)  vx *= -1;
    if (y <= 0 || y >= height) vy *= -1;
}

std::string Ball::toString() const {
    std::ostringstream ss;
    ss << x << "," << y;
    return ss.str();
}
