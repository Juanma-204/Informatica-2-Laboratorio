#pragma once

#include <string>
#include <map>
#include <vector>
#include <utility>

// Definicion de INFINITO (Se puede ajustar segun el rango de costos)
#define INFINITO 999999

using TablaDistancias = std::map<std::string, std::pair<int, std::string>>; // <Destino, <Costo, ProximoSalto>>
using Vecinos = std::map<std::string, int>; // <IDVecino, Costo>

class Router {
private:
    std::string id;
    Vecinos vecinos;
    TablaDistancias tablaDistancias;

public:
    // Constructor
    Router(const std::string& id) : id(id) {}

    // Metodos de gestion de vecinos
    void agregarVecino(const std::string& vecinoId, int costo);
    void eliminarVecino(const std::string& vecinoId);

    // Getters CONST (CRUCIAL para los errores que tienes)
    std::string getId() const { return id; }
    const Vecinos& getVecinos() const { return vecinos; }
    const TablaDistancias& getTablaDistancias() const { return tablaDistancias; }

    // Metodos para DV
    void inicializarTablaDistancias(const std::map<std::string, int>& todosLosRouters);
    bool actualizarTablaDistancias(const Router& routerVecino);
};
