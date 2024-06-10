#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>

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
            
            //string fileNameAux = splitter(splitter(fileName, '/')[1], '.')[0];
            
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