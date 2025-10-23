#include "red.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <map>
#include <iterator> // Necesario para usar std::next

using namespace std;

// Función auxiliar para imprimir el camino (sin tildes)
void imprimirCamino(const list<string>& camino, int costo) {
    if (camino.empty() || costo == INFINITO) {
        cout << "Camino no disponible o inaccesible." << endl;
        return;
    }

    cout << "Ruta: ";
    for (auto it = camino.begin(); it != camino.end(); ++it) {
        cout << *it;
        // Uso de std::next para evitar el " -> " despues del ultimo nodo
        if (std::next(it) != camino.end()) {
            cout << " -> ";
        }
    }
    cout << endl;
    cout << "Costo total: " << costo << endl;
}

// Función principal para la lógica de la aplicación
int main() {
    Red miRed;
    string nombreArchivo;

    // 1. Pedir el nombre del archivo de topologia
    cout << "\n=== SIMULADOR DE PROTOCOLO DISTANCIA VECTORIAL (DV) ===\n" << endl;
    cout << "Ingrese el nombre del archivo de topologia (ej: topologia.txt): ";
    getline(cin, nombreArchivo);

    if (nombreArchivo.empty()) {
        cerr << "Error: Nombre de archivo no proporcionado. Saliendo." << endl;
        return 1;
    }

    // 2. Cargar el archivo y simular
    if (!miRed.cargarDesdeArchivo(nombreArchivo)) {
        cerr << "Saliendo del programa debido a un error de carga o archivo faltante." << endl;
        return 1;
    }

    // 3. Simular el algoritmo
    if (miRed.getEnrutadores().size() > 0) {
        cout << "Topologia cargada. Ejecutando simulacion de enrutamiento..." << endl;
        miRed.simularEnrutamiento();
        cout << "Simulacion completada." << endl;
    } else {
        cerr << "Error: La red esta vacia despues de la carga." << endl;
        return 1;
    }

    // 4. Bucle interactivo
    string lineaEntrada, comando, param1, param2, param3;

    cout << "\n\n--- INTERFAZ DE CONSULTA ---" << endl;
    cout << "Comandos disponibles:" << endl;
    cout << " T <ID>          : Mostrar Tabla de Distancias para el Router <ID>." << endl;
    cout << " C <Origen> <Destino> : Mostrar Camino Mas Corto entre Origen y Destino." << endl;
    cout << " A <Origen> <Destino> <Costo>: Agregar/Modificar Conexion." << endl;
    cout << " E <ID>          : Eliminar Router <ID> y conexiones." << endl;
    cout << " S               : Salir." << endl;
    cout << "----------------------------" << endl;

    while (true) {
        cout << "\nConsulta (T, C, A, E, S): ";
        getline(cin, lineaEntrada);
        stringstream ss(lineaEntrada);

        if (!(ss >> comando)) {
            continue; // Linea vacia
        }

        if (comando == "S" || comando == "s") {
            break;
        }

        try {
            if (comando == "T" || comando == "t") {
                if (ss >> param1) {
                    miRed.mostrarTablaDistancias(param1);
                } else {
                    cout << "Error: Falta el ID del router para la tabla." << endl;
                }
            } else if (comando == "C" || comando == "c") {
                if (ss >> param1 && ss >> param2) {
                    list<string> camino = miRed.obtenerCaminoMasCorto(param1, param2);

                    // Necesitamos calcular el costo fuera de la funcion obtenerCaminoMasCorto
                    // Asumiremos que el costo esta en el primer par de la tabla del origen
                    const auto& enrutadores = miRed.getEnrutadores();
                    int costo = INFINITO;

                    if (enrutadores.count(param1)) {
                        const auto& tabla = enrutadores.at(param1).getTablaDistancias();
                        if (tabla.count(param2)) {
                            costo = tabla.at(param2).first;
                        }
                    }

                    imprimirCamino(camino, costo);
                } else {
                    cout << "Error: Faltan Origen y/o Destino para la consulta de camino." << endl;
                }
            } else if (comando == "A" || comando == "a") {
                if (ss >> param1 && ss >> param2 && ss >> param3) {
                    int costo = stoi(param3);
                    miRed.agregarConexion(param1, param2, costo);
                    miRed.simularEnrutamiento();
                    cout << "Conexion actualizada. Simulacion reiniciada." << endl;
                } else {
                    cout << "Error: Faltan Origen, Destino y Costo para agregar/modificar conexion." << endl;
                }
            } else if (comando == "E" || comando == "e") {
                if (ss >> param1) {
                    miRed.eliminarEnrutador(param1);
                    miRed.simularEnrutamiento();
                    cout << "Router " << param1 << " eliminado. Simulacion reiniciada." << endl;
                } else {
                    cout << "Error: Falta el ID del router a eliminar." << endl;
                }
            } else {
                cout << "Comando no reconocido. Use T, C, A, E o S." << endl;
            }
        } catch (const out_of_range& e) {
            cerr << "Error de rango: Router o destino no encontrado." << endl;
        } catch (const invalid_argument& e) {
            cerr << "Error de argumento: El costo debe ser un numero." << endl;
        } catch (const exception& e) {
            cerr << "Error inesperado: " << e.what() << endl;
        }
    }

    cout << "\nSaliendo del simulador. ¡Hasta pronto!" << endl;
    return 0;
}
