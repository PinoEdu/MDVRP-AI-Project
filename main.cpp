#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <random>
#include <unordered_set>

using namespace std;

const string FILE_NAME = "Instancias Conocidas/p04.txt";
const int SEED = 12345;

// Estructura para representar un cliente
struct Cliente {
    int id, tiempoServicio, demanda, frecuenciaVisita, numCombinacionesVisita;
    double x, y;
    vector<int> depositosVisitantes; // NO SE USA (SOLO SE GUARDAN)
};

// Estructura para representar un deposito
struct Deposito {
    int id, tiempoServicio, demanda, frecuenciaVisita, numCombinacionesVisita, distanciaMaxima, capacidadMaxima;
    double x, y;
};

// Estructura para representar una ruta
struct Ruta {
    int depositoId;
    vector<int> clientesVisitados;
    int cargaActual = 0;
    double distanciaRecorrida = 0;
};

// Estructura para representar todas las componentes
struct Element {
    vector<Deposito> depositos;
    vector<Cliente> clientes;
    int type, M, N, T;
    string fileName;
};

// Función para imprimir resultados
void printResults(const vector<Ruta>& rutas, const Element& elements) {
    double distanciaTotal = 0;
    cout << "Rutas generadas:" << endl;

    for (int i = 0; i < elements.M * elements.T; ++i) {
        cout << "Ruta del vehículo " << (i % elements.M) + 1 << " del depósito " << rutas[i].depositoId << ":" << endl;
        cout << "   Clientes visitados(" + to_string(rutas[i].clientesVisitados.size()) + "): ";
        for (int cliente : rutas[i].clientesVisitados) {
            cout << cliente << " ";
        }
        cout << endl;
        cout << "   Carga actual: " << rutas[i].cargaActual << endl;
        cout << "   Distancia recorrida: " << rutas[i].distanciaRecorrida << endl;
        distanciaTotal += rutas[i].distanciaRecorrida;
        cout << endl; // Separar las rutas con una línea en blanco
    }
    cout << "Distancia total recorrida: " << distanciaTotal << endl;
}

// Función para imprimir clientes
void printClients(const vector<Cliente>& clientes) {
    cout << "Clientes:" << endl;
    for (const Cliente& cliente : clientes) {
        cout << cliente.id << " ";
        cout << cliente.x << " ";
        cout << cliente.y << " ";
        cout << cliente.tiempoServicio << " ";
        cout << cliente.demanda << " ";
        cout << cliente.frecuenciaVisita << " ";
        cout << cliente.numCombinacionesVisita << " ";
        for (int deposito : cliente.depositosVisitantes) {
            cout << deposito << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Funcion para crear el archivo de salida
void saveResults(const vector<Ruta>& rutas, const Element& elements) {
    ofstream outputFile(elements.fileName + "_" + to_string(SEED) +".out");
    if(outputFile.is_open()) {
        double distanciaTotal = 0;
        for (const Ruta& ruta : rutas) {
            distanciaTotal += ruta.distanciaRecorrida;
        }

        outputFile << floor(distanciaTotal * 100) / 100 << endl;    // Trunco a dos decimales

        for(int i = 0; i < elements.M * elements.T; ++i) {
            // Solo escribir rutas con clientes
            if (!rutas[i].clientesVisitados.empty()) {
                outputFile << rutas[i].depositoId << " ";
                outputFile << (i % elements.M) + 1 << " ";
                outputFile << floor(rutas[i].distanciaRecorrida * 100) / 100 << " "; // Trunco a dos decimales
                outputFile << rutas[i].cargaActual << " ";
                outputFile << "0" << " ";       // Agregar el deposito de comienzo
                for (int cliente : rutas[i].clientesVisitados) {
                    outputFile << cliente << " ";
                }
                outputFile << "0" << endl;      // Agregar el deposito de llegada
            }
        }

        outputFile.close();
    } else {
        cerr << "No se pudo crear el archivo de salida." << endl;
    }
}

int getSizeTL(const vector<Ruta>& rutas) {
    float largo = 0.0;
    for(const Ruta& ruta: rutas) {
        largo += ruta.clientesVisitados.size();
    }
    return int(largo/rutas.size());
}

int getClientsCount(const vector<Ruta>& rutas) {
    float largo = 0.0;
    for(const Ruta& ruta: rutas) {
        largo += ruta.clientesVisitados.size();
    }
    return largo;
}

vector<Cliente> clientsSwap(const vector<Cliente>& clientes, mt19937& gen) {
    vector<Cliente> clientesCopia = clientes; 

    uniform_int_distribution<> distCliente(0, clientesCopia.size() - 1);
    int cliente1Index = distCliente(gen);
    int cliente2Index = distCliente(gen);

    // Asegurar que los indices sean diferentes
    while (cliente1Index == cliente2Index) {
        cliente2Index = distCliente(gen);
    }

    // Se realiza el swap de todos los elementos del struct, menos del ID
    swap(clientesCopia[cliente1Index].x, clientesCopia[cliente2Index].x);
    swap(clientesCopia[cliente1Index].y, clientesCopia[cliente2Index].y);
    swap(clientesCopia[cliente1Index].tiempoServicio, clientesCopia[cliente2Index].tiempoServicio);
    swap(clientesCopia[cliente1Index].demanda, clientesCopia[cliente2Index].demanda);
    swap(clientesCopia[cliente1Index].frecuenciaVisita, clientesCopia[cliente2Index].frecuenciaVisita);
    swap(clientesCopia[cliente1Index].numCombinacionesVisita, clientesCopia[cliente2Index].numCombinacionesVisita);
    swap(clientesCopia[cliente1Index].depositosVisitantes, clientesCopia[cliente2Index].depositosVisitantes);

    return clientesCopia;
}

vector<string> splitter(string str, char pattern) {
    vector<string> splitVector;
    stringstream ss(str); 
    string item;

    while (getline(ss, item, pattern)) {
        item.erase(0, item.find_first_not_of(' '));
        item.erase(item.find_last_not_of(' ') + 1);
        if (!item.empty()) {
            splitVector.push_back(item);
        }
    }

    // Agregar el ultimo elemento si la cadena no termina en el patron
    if (!ss.eof()) { 
        item = ss.str().substr(ss.tellg());
        item.erase(0, item.find_first_not_of(' '));
        item.erase(item.find_last_not_of(' ') + 1);
        if (!item.empty()) {
            splitVector.push_back(item);
        }
    }
    return splitVector;
}

// Funcion para calcular la distancia euclidiana entre dos puntos
double euclideanDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Funcion que aplica el algoritmo Greedy al problema
vector<Ruta> greedy(int M, int N, int T, const vector<Deposito>& depositos, const vector<Cliente>& clientes) {
    vector<Ruta> rutas(M * T);
    for (int i = 0; i < M * T; ++i) {
        rutas[i].depositoId = i / M + 1;
    }

    vector<bool> clienteVisitado(N, false);

    for (Ruta& ruta : rutas) {
        int depositoActual = ruta.depositoId - 1;
        double xDeposito = depositos[depositoActual].x;
        double yDeposito = depositos[depositoActual].y;
        int clienteActual = -1;

        while (true) {
            int clienteMasCercano = -1;
            double distanciaMinima = numeric_limits<double>::max();

            for (int j = 0; j < N; ++j) {
                // Verificar si el cliente cumple con todas las restricciones
                if (!clienteVisitado[j] &&
                    find(clientes[j].depositosVisitantes.begin(), clientes[j].depositosVisitantes.end(), depositoActual + 1) != clientes[j].depositosVisitantes.end() &&
                    ruta.cargaActual + clientes[j].demanda <= depositos[depositoActual].capacidadMaxima) {

                    double distancia;
                    if (clienteActual == -1) {
                        distancia = euclideanDistance(xDeposito, yDeposito, clientes[j].x, clientes[j].y);
                    } else {
                        distancia = euclideanDistance(clientes[clienteActual].x, clientes[clienteActual].y, clientes[j].x, clientes[j].y);
                    }

                    // Verificar si la distancia acumulada no supera la distancia maxima del deposito
                    if (distancia + ruta.distanciaRecorrida <= depositos[depositoActual].distanciaMaxima || depositos[depositoActual].distanciaMaxima == 0) {
                        if (distancia < distanciaMinima) {
                            distanciaMinima = distancia;
                            clienteMasCercano = j;
                        }
                    }
                }
            }

            if (clienteMasCercano == -1) {
                if (clienteActual != -1) { // Solo agregar distancia de vuelta si se visito algun cliente
                    ruta.distanciaRecorrida += euclideanDistance(xDeposito, yDeposito, clientes[clienteActual].x, clientes[clienteActual].y);
                }
                break;
            }

            clienteVisitado[clienteMasCercano] = true;
            ruta.clientesVisitados.push_back(clienteMasCercano + 1);
            ruta.cargaActual += clientes[clienteMasCercano].demanda;
            ruta.distanciaRecorrida += distanciaMinima;
            clienteActual = clienteMasCercano;
        }
    }
    return rutas;
}

// Funcion para evaluar una solución (distancia total)
double evaluateSolution(const vector<Ruta>& rutas) {
    double distanciaTotal = 0;
    for (const Ruta& ruta : rutas) {
        distanciaTotal += ruta.distanciaRecorrida;
    }
    return distanciaTotal;
}

// Función para realizar el movimiento Swap (intercambio de clientes entre rutas o dentro de la misma ruta)
bool swapMovement(vector<Ruta>& rutas, vector<Cliente>& clientes, const vector<Deposito>& depositos, mt19937& gen, unordered_set<string>& tabuList) {
    uniform_int_distribution<> distRuta(0, rutas.size() - 1);
    uniform_int_distribution<> distTipoSwap(0, 1); // 0: Intra-ruta, 1: Inter-rutas

    int tipoSwap = distTipoSwap(gen);
    int ruta1Index, ruta2Index, cliente1Index, cliente2Index;
    Ruta ruta1, ruta2;

    if (tipoSwap == 0) { // Swap dentro de la misma ruta
        // Elegir una ruta aleatoriamente, asegurandose de que tenga al menos dos clientes
        do {
            ruta1Index = distRuta(gen);
        } while (rutas[ruta1Index].clientesVisitados.size() < 2);

        ruta2Index = ruta1Index; // Ambas rutas son la misma
        ruta1 = rutas[ruta1Index];
        ruta2 = rutas[ruta1Index];

        // Elegir dos clientes aleatoriamente de la ruta seleccionada
        uniform_int_distribution<> distCliente(0, ruta1.clientesVisitados.size() - 1);
        cliente1Index = distCliente(gen);
        cliente2Index = distCliente(gen);
        while (cliente1Index == cliente2Index) { // Asegurar que los clientes sean diferentes
            cliente2Index = distCliente(gen);
        }
    } else { // Swap entre rutas diferentes
        // Elegir dos rutas aleatoriamente, asegurandose de que no sean la misma y que tengan clientes
        do {
            ruta1Index = distRuta(gen);
            ruta2Index = distRuta(gen);
        } while (ruta1Index == ruta2Index || rutas[ruta1Index].clientesVisitados.empty() || rutas[ruta2Index].clientesVisitados.empty());

        ruta1 = rutas[ruta1Index];
        ruta2 = rutas[ruta2Index];

        // Elegir dos clientes aleatoriamente de las rutas seleccionadas
        uniform_int_distribution<> distCliente1(0, ruta1.clientesVisitados.size() - 1);
        uniform_int_distribution<> distCliente2(0, ruta2.clientesVisitados.size() - 1);
        cliente1Index = distCliente1(gen);
        cliente2Index = distCliente2(gen);
    }

    // Obtener los indices de los clientes en el vector original
    int cliente1Id = rutas[ruta1Index].clientesVisitados[cliente1Index] - 1; // Indice 0-indexado
    int cliente2Id = rutas[ruta2Index].clientesVisitados[cliente2Index] - 1; // Indice 0-indexado

    // Verificar si el movimiento esta en la lista tabu
    string tabuKey = to_string(cliente1Id) + "_" + to_string(cliente2Id);
    if (tabuList.find(tabuKey) != tabuList.end()) {
        return false; // Movimiento tabu, no se realiza
    }

    // Intercambiar los clientes en las rutas
    swap(rutas[ruta1Index].clientesVisitados[cliente1Index], rutas[ruta2Index].clientesVisitados[cliente2Index]);

    // Verificar restricciones despues del intercambio (capacidad y distancia)
    bool restriccionesCumplidas = true;
    for (int i = 0; i < 2; ++i) { // Iterar sobre las rutas seleccionadas
        int rutaIndex = (i == 0) ? ruta1Index : ruta2Index; // Indice de la ruta actual
        Ruta& ruta = rutas[rutaIndex]; // Obtener la ruta por referencia

        int depositoActual = ruta.depositoId - 1;
        ruta.cargaActual = 0;
        ruta.distanciaRecorrida = 0;

        for (int j = 0; j < ruta.clientesVisitados.size(); ++j) {
            int cliente = ruta.clientesVisitados[j] - 1; // Indice 0-indexado
            ruta.cargaActual += clientes[cliente].demanda;

            // Calcular distancia desde el deposito o el cliente anterior
            double distancia = (j == 0) ?
                euclideanDistance(depositos[depositoActual].x, depositos[depositoActual].y, clientes[cliente].x, clientes[cliente].y) :
                euclideanDistance(clientes[ruta.clientesVisitados[j - 1] - 1].x, clientes[ruta.clientesVisitados[j - 1] - 1].y, clientes[cliente].x, clientes[cliente].y);
            ruta.distanciaRecorrida += distancia;

            // Verificar restricciones
            if (ruta.cargaActual > depositos[depositoActual].capacidadMaxima ||
                (depositos[depositoActual].distanciaMaxima > 0 && ruta.distanciaRecorrida > depositos[depositoActual].distanciaMaxima)) {
                restriccionesCumplidas = false;
                break;
            }
        }

        // Agregar distancia de regreso al depósito
        if (!ruta.clientesVisitados.empty()) {
            int ultimoCliente = ruta.clientesVisitados.back() - 1;
            ruta.distanciaRecorrida += euclideanDistance(clientes[ultimoCliente].x, clientes[ultimoCliente].y, depositos[depositoActual].x, depositos[depositoActual].y);
        }

        if (!restriccionesCumplidas) {
            break; // Si alguna ruta no cumple las restricciones, deshacer el swap
        }
    }

    // Deshacer el swap si las restricciones no se cumplen
    if (!restriccionesCumplidas) {
        swap(ruta1.clientesVisitados[cliente1Index], ruta2.clientesVisitados[cliente2Index]);
        return false;
    }

    // Agregar el movimiento a la lista tabu
    tabuList.insert(tabuKey);
    return true; // Swap realizado con exito
}

vector<Ruta> tabuSearch(vector<Ruta> rutas, vector<Cliente>& clientes, const vector<Deposito>& depositos, mt19937& gen, int tabuTenure, int maxIteraciones) {
    unordered_set<string> tabuList;
    double mejorDistancia = evaluateSolution(rutas);
    vector<Ruta> mejorSolucion = rutas;
    int iteracion = 0;
    int iteracionSinMejora = 0;
    const int maxIteracionesSinMejora = 10; // Criterio de parada por estancamiento

    while (iteracion < maxIteraciones && iteracionSinMejora < maxIteracionesSinMejora) {
        bool swapRealizado = swapMovement(rutas, clientes, depositos, gen, tabuList);

        if (swapRealizado) {
            double nuevaDistancia = evaluateSolution(rutas);

            // Criterio de aspiración: Aceptar el movimiento incluso si es tabú si mejora la mejor solución
            if (nuevaDistancia < mejorDistancia) { 
                mejorDistancia = nuevaDistancia;
                mejorSolucion = rutas;
                tabuList.clear(); // Vaciar la lista tabu si se encuentra una mejor solucion
                iteracionSinMejora = 0; // Reiniciar el contador de estancamiento
            } else {
                iteracionSinMejora++;
            }
        } else {
            iteracionSinMejora++;
        }

        // Actualizar la lista tabu
        if (tabuList.size() >= tabuTenure) {
            tabuList.erase(tabuList.begin());
        }

        iteracion++;
    }

    return mejorSolucion;
}

Element decompressor(string fileName) {
    ifstream myFile(fileName);

    string linea;
    vector<string> splitVector;

    if (myFile.is_open()) { 
        try {
            getline(myFile, linea);
            splitVector = splitter(linea, ' ');

            int type = stoi(splitVector[0]);// Tipo de problema
            int M = stoi(splitVector[1]);   // Numero de vehiculos por deposito
            int N = stoi(splitVector[2]);   // Numero de clientes
            int T = stoi(splitVector[3]);   // Numero de depositos

            vector<Deposito> depositos(T);
            vector<Cliente> clientes(N);

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                depositos[i].distanciaMaxima = stoi(splitVector[0]);
                depositos[i].capacidadMaxima = stoi(splitVector[1]);
            }

            for (int i = 0; i < N; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                clientes[i].id = stoi(splitVector[0]);
                clientes[i].x = stof(splitVector[1]);
                clientes[i].y = stof(splitVector[2]);
                clientes[i].tiempoServicio = stoi(splitVector[3]);
                clientes[i].demanda = stoi(splitVector[4]);
                clientes[i].frecuenciaVisita = stoi(splitVector[5]);
                clientes[i].numCombinacionesVisita = stoi(splitVector[6]);
                for(int j = 7; j < splitVector.size(); j++) {
                    int valor = stoi(splitVector[j]);
                    clientes[i].depositosVisitantes.push_back(valor);
                }
            }

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                depositos[i].id = stoi(splitVector[0]);
                depositos[i].x = stof(splitVector[1]);
                depositos[i].y = stof(splitVector[2]);
                depositos[i].tiempoServicio = stoi(splitVector[3]);
                depositos[i].demanda = stoi(splitVector[4]);
                depositos[i].frecuenciaVisita = stoi(splitVector[5]);
                depositos[i].numCombinacionesVisita = stoi(splitVector[6]);
            }

            Element elements;

            elements.clientes = clientes;
            elements.depositos = depositos;
            elements.fileName = splitter(splitter(fileName, '/')[1], '.')[0];
            elements.type = type;
            elements.M = M;
            elements.N = N;
            elements.T = T;

            return elements;
        } catch (const ifstream::failure& e) {
            cerr << "Error al abrir o leer el archivo: " << e.what() << endl;
        } catch (const invalid_argument& e) {
            cerr << "Error al convertir valores: " << e.what() << endl;
        } catch (const out_of_range& e) {
            cerr << "Valor fuera de rango: " << e.what() << endl;
        } catch (const runtime_error& e) {
            cerr << "Error en el formato del archivo: " << e.what() << endl;
        }
        myFile.close(); 
    } else {
        cerr << "No se pudo abrir el archivo " << "'" + fileName + "'" << endl;
    }
    Element elements;
    return elements;
}

int main() { 
    mt19937 gen(SEED);

    Element elements;
    elements = decompressor(FILE_NAME);

    int numSwaps = int(elements.clientes.size()/2);     // Se castea a int() en caso de que el largo da decimal
    for(int i = 0; i < numSwaps; i++) {
        elements.clientes = clientsSwap(elements.clientes, gen);
    }

    vector<Ruta> rutas(elements.M * elements.T);
    rutas = greedy(elements.M, elements.N, elements.T, elements.depositos, elements.clientes);

    int tabuListSize = getSizeTL(rutas);
    //cout << "Largo lista tabu: " << tabuListSize << endl;

    vector<Ruta> mejorSolucion = tabuSearch(rutas, elements.clientes, elements.depositos, gen, tabuListSize, 100); 

    //cout << "Se visitaron " << getClientsCount(rutas) << " utilizando Greedy" << endl;
    //cout << "Se visitaron " << getClientsCount(mejorSolucion) << " utilizando Tabu Search" << endl;

    //printResults(rutas, elements);
    saveResults(mejorSolucion, elements);
    return 0;
};