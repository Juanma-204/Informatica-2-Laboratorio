#pragma once

#include "router.h" // Incluye Router y la definicion de INFINITO
#include <string>
#include <map>
#include <list>

using Enrutadores = std::map<std::string, Router>;

class Red {
private:
    // La variable 'enrutadores' debe estar declarada AQUI como miembro privado
    Enrutadores enrutadores;

    void agregarEnrutador(const std::string& id);

public:
    // Metodos principales
    bool cargarDesdeArchivo(const std::string& nombreArchivo);
    void simularEnrutamiento();

    // Getters CONST (CRUCIAL para corregir errores de 'enrutadores' en main.cpp)
    const Enrutadores& getEnrutadores() const { return enrutadores; }

    // Metodos de consulta CONST (CRUCIAL para el error 'ov-qualifier')
    void mostrarTablaDistancias(const std::string& idRouter) const;
    std::list<std::string> obtenerCaminoMasCorto(const std::string& origen, const std::string& destino) const;

    // Metodos de modificacion
    void eliminarEnrutador(const std::string& id);
    void agregarConexion(const std::string& origen, const std::string& destino, int costo);
};
