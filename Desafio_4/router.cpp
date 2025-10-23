#include "router.h"
#include <algorithm>
#include <list>
#include <iostream>
#include <map>

// Implementacion de agregarVecino
void Router::agregarVecino(const std::string& vecinoId, int costo) {
    vecinos[vecinoId] = costo;
}

// Implementacion de eliminarVecino
void Router::eliminarVecino(const std::string& vecinoId) {
    // Eliminar de la lista de vecinos directos
    vecinos.erase(vecinoId);

    // Invalidar entradas de la tabla que usaban a este vecino como proximo salto
    for (auto& par : tablaDistancias) {
        if (par.second.second == vecinoId) {
            // Reinicializar la ruta a infinito y sin proximo salto
            par.second.first = INFINITO;
            par.second.second = "";
        }
    }
}

// ----------------------------------------------------
// Metodos de Enrutamiento (Distance Vector)
// ----------------------------------------------------

// 1. Inicializa la tabla con los costos directos a vecinos e infinito al resto
void Router::inicializarTablaDistancias(const std::map<std::string, int>& todosLosRouters) {
    tablaDistancias.clear();

    // Insertar todos los routers en la tabla
    for (const auto& par : todosLosRouters) {
        const std::string& destinoId = par.first;

        if (destinoId == id) {
            // Costo a si mismo es 0, proximo salto es uno mismo
            tablaDistancias[destinoId] = {0, id};
        } else if (vecinos.count(destinoId)) {
            // Costo directo al vecino, proximo salto es el vecino
            tablaDistancias[destinoId] = {vecinos.at(destinoId), destinoId};
        } else {
            // Costo a router desconocido es infinito, sin proximo salto
            tablaDistancias[destinoId] = {INFINITO, ""};
        }
    }
}

// 2. Actualiza la tabla basado en el Distance Vector del vecino
bool Router::actualizarTablaDistancias(const Router& vecino) {
    bool tablaCambio = false; // Variable local para rastrear si hubo cambios

    // El costo hasta el vecino
    // Usamos at() porque el router SIEMPRE debe ser vecino de si mismo para este proceso
    // (a menos que haya una desconexion, manejado en otro lugar)
    int costoA_Vecino = vecinos.at(vecino.getId());

    // Recorrer la tabla de distancias que el vecino nos ha enviado
    for (const auto& parDestino : vecino.getTablaDistancias()) {
        const std::string& destino = parDestino.first;
        int costoVecino_a_Destino = parDestino.second.first;

        // El router no necesita una ruta hacia sí mismo a través de otro nodo
        if (destino == id) {
            continue;
        }

        // Si el vecino tiene un camino válido hacia el destino
        if (costoVecino_a_Destino != INFINITO) {
            // Calcular el nuevo costo total a traves del vecino
            // Es seguro sumar si costoVecino_a_Destino no es INFINITO
            int nuevoCostoTotal = costoA_Vecino + costoVecino_a_Destino;

            // 1. Condicion de mejora (es más corto)
            // 2. Condicion de loopback/split horizon (usamos el mismo camino que el vecino)
            //    Si el proximo salto actual es el vecino, debemos aceptar la actualizacion aunque no sea un cambio,
            //    por si el costo de la ruta del vecino ha cambiado (ej: se ha roto un enlace, o mejorado).

            int costoActual = tablaDistancias.at(destino).first;
            std::string proximoSaltoActual = tablaDistancias.at(destino).second;

            if (nuevoCostoTotal < costoActual) {
                // Actualizar la entrada: encontramos un camino más corto
                tablaDistancias[destino] = {nuevoCostoTotal, vecino.getId()};
                tablaCambio = true;
            } else if (proximoSaltoActual == vecino.getId() && nuevoCostoTotal != costoActual) {
                // Split Horizon: si el proximo salto actual es el vecino, y el costo cambio,
                // debemos actualizar nuestra ruta.
                tablaDistancias[destino] = {nuevoCostoTotal, vecino.getId()};
                tablaCambio = true;
            }
        }
    }

    return tablaCambio;
}
