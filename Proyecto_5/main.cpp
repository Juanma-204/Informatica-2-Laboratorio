#include "simulador.h"
#include <iostream>

int main() {
    int n;
    std::cout << "Ingrese el numero de particulas: ";
    std::cin >> n;

    Simulador sim(n, 1000, 120);
    sim.ejecutar();

    return 0;
}
