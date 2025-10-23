#include "red.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility> // Para std::pair
#include <list>
#include <map>

// ----------------------------------------------------
// Metodos Auxiliares
// ----------------------------------------------------

/**
 * @brief Metodo auxiliar que agrega un Router a la red si no existe.
 * @param id ID del router a agregar.
 */
void Red::agregarEnrutador(const std::string& id) {
    // Si no encontramos el ID en el mapa, lo agregamos.
    if (enrutadores.find(id) == enrutadores.end()) {
        enrutadores.emplace(id, Router(id));
    }
}

// ----------------------------------------------------
// Metodos Principales de Red
// ----------------------------------------------------

/**
 * @brief Carga la topologia de la red desde un archivo de texto.
 * @param nombreArchivo Nombre del archivo de topologia.
 * @return true si la carga fue exitosa, false en caso contrario.
 */
bool Red::cargarDesdeArchivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        // Usa cerr para mensajes de error
        std::cerr << "Error: No se pudo abrir el archivo de topologia: " << nombreArchivo << std::endl;
        return false;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string origenId, destinoId;
        int costo;

        // Formato: Origen Destino Costo
        if (ss >> origenId >> destinoId >> costo) {
            // 1. Asegurar que ambos routers existan en la red
            agregarEnrutador(origenId);
            agregarEnrutador(destinoId);

            // 2. Agregar conexion bidireccional a nivel de Router
            enrutadores.at(origenId).agregarVecino(destinoId, costo);
            enrutadores.at(destinoId).agregarVecino(origenId, costo);
        } else if (!linea.empty()) {
            std::cerr << "Advertencia: Linea de archivo con formato incorrecto: " << linea << std::endl;
        }
    }
    archivo.close();

    // Despues de cargar la topologia, inicializar todas las tablas de distancia
    // Se crea una lista de todos los IDs de routers en la red
    std::map<std::string, int> todosLosRouters;
    for (const auto& par : enrutadores) {
        todosLosRouters[par.first] = 0;
    }

    // Inicializar cada tabla con los IDs de todos los routers
    for (auto& par : enrutadores) {
        par.second.inicializarTablaDistancias(todosLosRouters);
    }

    return true;
}

/**
 * @brief Simula el algoritmo de enrutamiento (Distancia Vectorial) hasta que converge.
 */
void Red::simularEnrutamiento() {
    std::cout << "\n--- Iniciando Simulacion de Enrutamiento (Distancia Vectorial) ---" << std::endl;
    bool convergencia = false;
    int iteraciones = 0;

    while (!convergencia) {
        convergencia = true;
        iteraciones++;
        bool cambioEnEstaIteracion = false;

        // Usamos una copia de los routers para simular que todos anuncian su vector al mismo tiempo
        // Esto evita que las actualizaciones de R1 afecten la decision de R2 en la misma iteracion.
        Enrutadores routersTemporales = enrutadores;

        // Iteramos sobre la copia para obtener los vectores de distancia a propagar
        for (auto& parOrigen : routersTemporales) {
            const std::string& idOrigen = parOrigen.first;
            Router& routerOrigen = enrutadores.at(idOrigen); // Router original para actualizar

            // Router Origen procesa la informacion de TODOS sus vecinos
            for (const auto& parVecino : routerOrigen.getVecinos()) {
                const std::string& idVecino = parVecino.first;

                if (routersTemporales.count(idVecino)) {
                    // El routerOrigen actualiza su tabla usando el vector de distancia de su vecino 'idVecino'
                    // El metodo actualizarTablaDistancias retorna true si hubo algun cambio
                    bool cambio = routerOrigen.actualizarTablaDistancias(routersTemporales.at(idVecino));
                    if (cambio) {
                        cambioEnEstaIteracion = true;
                    }
                }
            }
        }

        if (cambioEnEstaIteracion) {
            convergencia = false;
        }

        // Parada de seguridad para evitar loops infinitos (por si el algoritmo no converge)
        if (iteraciones > 100) {
            std::cerr << "Advertencia: Simulacion detenida despues de 100 iteraciones (posible bucle/convergencia lenta)." << std::endl;
            break;
        }
    }

    std::cout << "Simulacion finalizada. Convergencia en " << iteraciones << " iteraciones." << std::endl;
}

/**
 * @brief Muestra la tabla de distancias de un router especifico.
 * @param idRouter ID del router cuya tabla se va a mostrar.
 */
void Red::mostrarTablaDistancias(const std::string& idRouter) const {
    // Es CONST, solo podemos usar getTablaDistancias() y getVecinos() que tambien son const.
    if (enrutadores.count(idRouter)) {
        const Router& router = enrutadores.at(idRouter);
        std::cout << "\n--- Tabla de Distancias del Router " << router.getId() << " ---" << std::endl;
        std::cout << "Destino\t|\tCosto\t|\tProximo Salto" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;

        for (const auto& par : router.getTablaDistancias()) {
            const std::string& destino = par.first;
            int costo = par.second.first;
            const std::string& proximoSalto = par.second.second;

            // Imprimir el costo y el proximo salto
            std::cout << destino << "\t|\t";
            if (costo == INFINITO) {
                std::cout << "INF\t|\t";
            } else {
                std::cout << costo << "\t|\t";
            }
            std::cout << (proximoSalto.empty() ? "-" : proximoSalto) << std::endl;
        }
    } else {
        std::cerr << "Error: Router " << idRouter << " no existe en la red." << std::endl;
    }
}

/**
 * @brief Obtiene el camino mas corto entre dos routers.
 * * NOTA: Esta funcion reconstruye el camino saltando de router en router.
 * @param origen ID del router de origen.
 * @param destino ID del router de destino.
 * @return std::list<std::string> con los IDs de los routers en el camino.
 */
std::list<std::string> Red::obtenerCaminoMasCorto(const std::string& origen, const std::string& destino) const {
    std::list<std::string> camino;

    // Verificar existencia de routers
    if (!enrutadores.count(origen) || !enrutadores.count(destino)) {
        // main.cpp maneja el mensaje de error si no existen.
        return camino;
    }

    // Verificar si el origen y destino son el mismo
    if (origen == destino) {
        camino.push_back(origen);
        return camino;
    }

    // 1. Verificar si existe una ruta en la tabla del router de origen
    const Router& routerOrigen = enrutadores.at(origen);
    if (routerOrigen.getTablaDistancias().find(destino) == routerOrigen.getTablaDistancias().end() ||
        routerOrigen.getTablaDistancias().at(destino).first == INFINITO) {
        return camino; // Retorna lista vacia si no hay camino o el costo es infinito
    }

    // 2. Reconstruir el camino, nodo por nodo (siguiendo el next_hop de cada router)
    std::string actual = origen;

    // El bucle DEBE terminar cuando actual llegue al destino.
    while (actual != destino) {

        // 3. Verificamos que el nodo actual exista y tenga una entrada para el destino
        // Esto es necesario ya que se itera sobre los routers
        if (!enrutadores.count(actual) ||
            enrutadores.at(actual).getTablaDistancias().find(destino) == enrutadores.at(actual).getTablaDistancias().end()) {
            return {}; // Router intermedio ha desaparecido o no tiene ruta.
        }

        // 4. Agregamos el nodo actual al camino
        camino.push_back(actual);

        // 5. Obtenemos el proximo salto (next_hop) de la tabla del router ACTUAL hacia el DESTINO
        // Notar: usamos enrutadores.at(actual) para obtener la tabla del router actual, no la del origen.
        const std::string& proximoSalto = enrutadores.at(actual).getTablaDistancias().at(destino).second;

        if (proximoSalto.empty()) {
            // Si el next_hop está vacío, la ruta está rota.
            return {};
        }

        // 6. Si el proximo salto es el destino, lo agregamos y terminamos
        if (proximoSalto == destino) {
            camino.push_back(destino);
            return camino;
        }

        // 7. Si el proximo salto es el mismo nodo, hay un bucle (error de DV)
        if (proximoSalto == actual) {
            return {};
        }

        // 8. Movemos al proximo salto y seguimos iterando
        actual = proximoSalto;

        // Parada de seguridad (evitar bucles infinitos por error en el DV)
        if (camino.size() > enrutadores.size()) {
            return {};
        }
    }

    // Si la ruta se reconstruyo correctamente, el bucle deberia haber terminado.
    return camino;
}

// ----------------------------------------------------
// Metodos de Modificacion (Bonus)
// ----------------------------------------------------

/**
 * @brief Elimina un enrutador de la red.
 * @param id ID del router a eliminar.
 */
void Red::eliminarEnrutador(const std::string& id) {
    if (!enrutadores.count(id)) {
        std::cerr << "Error: No se puede eliminar. Router " << id << " no existe." << std::endl;
        return;
    }

    // 1. Notificar a todos los vecinos de su eliminacion (eliminando la conexion)
    Router& routerAEliminar = enrutadores.at(id);
    std::list<std::string> vecinosAfectados;
    // Recopilar los IDs de los vecinos antes de modificar el mapa de vecinos
    for (const auto& par : routerAEliminar.getVecinos()) {
        vecinosAfectados.push_back(par.first);
    }

    // Para cada vecino, eliminamos la conexion bidireccional
    for (const std::string& vecinoId : vecinosAfectados) {
        if (enrutadores.count(vecinoId)) {
            enrutadores.at(vecinoId).eliminarVecino(id);
        }
    }

    // 2. Eliminar el router de la coleccion
    enrutadores.erase(id);

    // 3. Re-inicializar tablas y re-simular
    std::map<std::string, int> todosLosRouters;
    for (const auto& par : enrutadores) {
        todosLosRouters[par.first] = 0;
    }

    for (auto& par : enrutadores) {
        par.second.inicializarTablaDistancias(todosLosRouters);
    }

    simularEnrutamiento();
    std::cout << "Router " << id << " eliminado. Simulacion re-iniciada." << std::endl;
}

/**
 * @brief Agrega o modifica una conexion entre dos routers.
 * @param origen ID del router de origen.
 * @param destino ID del router de destino.
 * @param costo Nuevo costo de la conexion.
 */
void Red::agregarConexion(const std::string& origen, const std::string& destino, int costo) {
    if (origen == destino) return;
    if (costo < 0) {
        std::cerr << "Error: El costo de la conexion debe ser no negativo." << std::endl;
        return;
    }

    // Asegurar que ambos routers existan
    agregarEnrutador(origen);
    agregarEnrutador(destino);

    // 1. Modificar la conexion bidireccional
    enrutadores.at(origen).agregarVecino(destino, costo);
    enrutadores.at(destino).agregarVecino(origen, costo);

    // 2. Re-inicializar tablas y re-simular
    std::map<std::string, int> todosLosRouters;
    for (const auto& par : enrutadores) {
        todosLosRouters[par.first] = 0;
    }

    for (auto& par : enrutadores) {
        par.second.inicializarTablaDistancias(todosLosRouters);
    }

    simularEnrutamiento();
    std::cout << "Conexion entre " << origen << " y " << destino << " modificada/agregada. Simulacion re-iniciada." << std::endl;
}
