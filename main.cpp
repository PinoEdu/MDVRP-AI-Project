#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

const int MDVRP_TYPE = 2;

// Estructura para representar un cliente
struct Cliente {
    int id, tiempoServicio, demanda, frecuenciaVisita, numCombinacionesVisita;
    double x, y;
    vector<int> depositosVisitantes; // Lista de depositos que pueden visitar
};

// Estructura para representar un deposito
struct Deposito {
    int id, tiempoServicio, demanda, frecuenciaVisita, numCombinacionesVisita, distanciaMaxima, capacidadMaxima;
    double x, y;
};

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

// Estructura para representar una ruta
struct Ruta {
    int depositoId;
    vector<int> clientesVisitados;
    int cargaActual = 0;
    double distanciaRecorrida = 0;
};

// Función para calcular la distancia euclidiana entre dos puntos
double calcularDistancia(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void solver(string fileName) {
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

            if (type != MDVRP_TYPE) {
                throw runtime_error("Tipo de problema incorrecto. Se espera MDVRP (tipo 2).");
            }

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

            vector<Ruta> rutas(M * T); // Inicializa las rutas (M vehículos por T depósitos)
            for (int i = 0; i < M * T; ++i) {
                rutas[i].depositoId = i / M + 1; 
            }

            vector<bool> clienteVisitado(N, false); 

            // Algoritmo Greedy
            for (Ruta& ruta : rutas) { // Referencia para modificar directamente la ruta
                int depositoActual = ruta.depositoId - 1; // Índice del depósito (0-indexado)
                double xDeposito = depositos[depositoActual].x;
                double yDeposito = depositos[depositoActual].y;
                int clienteActual = -1; // Comienza en el deposito (sin cliente asignado)

                while (true) {
                    // Encuentra el cliente mas cercano no visitado que pueda ser atendido
                    int clienteMasCercano = -1;
                    double distanciaMinima = numeric_limits<double>::max();
                    for (int j = 0; j < N; ++j) {
                        if (!clienteVisitado[j] && 
                            find(clientes[j].depositosVisitantes.begin(), clientes[j].depositosVisitantes.end(), depositoActual + 1) != clientes[j].depositosVisitantes.end() &&
                            ruta.cargaActual + clientes[j].demanda <= depositos[depositoActual].capacidadMaxima) {
                            
                            double distancia;
                            if (clienteActual == -1) { // Si es el primer cliente, calcular desde el depósito
                                distancia = calcularDistancia(xDeposito, yDeposito, clientes[j].x, clientes[j].y);
                            } else { // Si no, calcular desde el cliente actual
                                distancia = calcularDistancia(clientes[clienteActual].x, clientes[clienteActual].y, clientes[j].x, clientes[j].y);
                            }

                            if (distancia < distanciaMinima) {
                                distanciaMinima = distancia;
                                clienteMasCercano = j;
                            }
                        }
                    }

                    // Si no se encuentra un cliente valido, termina la ruta
                    if (clienteMasCercano == -1) {
                        ruta.distanciaRecorrida += calcularDistancia(xDeposito, yDeposito, clientes[clienteActual].x, clientes[clienteActual].y); // Regreso al depósito
                        break;
                    }

                    // Asigna el cliente a la ruta
                    clienteVisitado[clienteMasCercano] = true;
                    ruta.clientesVisitados.push_back(clienteMasCercano + 1); // +1 para indices 1-indexados
                    ruta.cargaActual += clientes[clienteMasCercano].demanda;
                    ruta.distanciaRecorrida += distanciaMinima;
                    clienteActual = clienteMasCercano;
                }
            }

            string fileNameAux = splitter(splitter(fileName, '/')[1], '.')[0];

            // Imprimir rutas y calcular distancia total
            double distanciaTotal = 0;
                cout << "Rutas generadas por el algoritmo Greedy:" << endl;

            for (int i = 0; i < M * T; ++i) {
                cout << "Ruta del vehículo " << (i % M) + 1 << " del depósito " << rutas[i].depositoId << ":" << endl;
                cout << "   Clientes visitados: ";
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
}

int main() { 
    solver("Instancias Conocidas/p04.txt");
    return 0;
};