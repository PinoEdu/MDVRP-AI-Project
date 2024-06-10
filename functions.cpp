#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

const int MDVRP_TYPE = 2;

// Estructura para representar un cliente
struct Cliente {
    int id, d, q, a, f;
    double x, y;
    vector<int> list;
};

// Estructura para representar un deposito
struct Deposito {
    int id, d, q, f, a, D, Q;
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

double calcularDistancia(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void greedyMDVRP(vector<Deposito>& depositos, vector<Cliente>& clientes, int M) {
    for (int i = 0; i < depositos.size(); i++) {
        vector<vector<Cliente>> rutas(M); // Inicializar rutas para cada vehículo
        double xDeposito = depositos[i].x;
        double yDeposito = depositos[i].y;

        for (int j = 0; j < clientes.size(); j++) {
            double distanciaMinima = numeric_limits<double>::max();
            int indiceMinimo = -1;

            for (int k = 0; k < rutas.size(); k++) {
                double distancia = calcularDistancia(xDeposito, yDeposito, clientes[j].x, clientes[j].y);
                if (clientes[j].q <= depositos[i].Q) {
                    if (distancia < distanciaMinima) {
                        distanciaMinima = distancia;
                        indiceMinimo = k;
                    }
                }
            }

            if (indiceMinimo != -1) {
                rutas[indiceMinimo].push_back(clientes[j]);
                depositos[i].Q -= clientes[j].q;
            }
        }

        // Imprimir las rutas
        cout << "Depósito " << depositos[i].id << ":" << endl;
        for (int j = 0; j < rutas.size(); j++) {
            cout << "Ruta " << j + 1 << ": ";
            for (const auto& cliente : rutas[j]) {
                cout << cliente.id << " ";
            }
            cout << endl;
        }
    }
}


void solver(string fileName) {
    ifstream myFile(fileName);

    string linea;
    vector<string> splitVector;

    if (myFile.is_open()) { 
        try {
            getline(myFile, linea);
            splitVector = splitter(linea, ' ');

            int type = stoi(splitVector[0]);
            int M = stoi(splitVector[1]);
            int N = stoi(splitVector[2]);
            int T = stoi(splitVector[3]);

            if (type != MDVRP_TYPE) {
                throw runtime_error("Tipo de problema incorrecto. Se espera MDVRP (tipo 2).");
            }

            vector<Deposito> depositos(T);
            vector<Cliente> clientes(N);

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                depositos[i].D = stoi(splitVector[0]);
                depositos[i].Q = stoi(splitVector[1]);
            }

            for (int i = 0; i < N; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                clientes[i].id = stoi(splitVector[0]);
                clientes[i].x = stof(splitVector[1]);
                clientes[i].y = stof(splitVector[2]);
                clientes[i].d = stoi(splitVector[3]);
                clientes[i].q = stoi(splitVector[4]);
                clientes[i].f = stoi(splitVector[5]);
                clientes[i].a = stoi(splitVector[6]);
                for(int j = 7; j < splitVector.size(); j++) {
                    int valor = stoi(splitVector[j]);
                    clientes[i].list.push_back(valor);
                }
            }

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                depositos[i].id = stoi(splitVector[0]);
                depositos[i].x = stof(splitVector[1]);
                depositos[i].y = stof(splitVector[2]);
                depositos[i].d = stoi(splitVector[3]);
                depositos[i].q = stoi(splitVector[4]);
                depositos[i].f = stoi(splitVector[5]);
                depositos[i].a = stoi(splitVector[6]);
            }
            
            string fileNameAux = splitter(splitter(fileName, '/')[1], '.')[0];

            greedyMDVRP(depositos, clientes, M);
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
    solver("Instancias Conocidas/p01.txt");
    return 0;
};