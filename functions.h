#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace std;

const int MDVRP_TYPE = 2;

struct Cliente {
    int id, demanda, tiempoServicio, frecuencia;
    double x, y; 
    vector<int> combinacionesVisita, ordenVisita;
};

struct Deposito {
    int duracionMaxima, capacidadMaxima;
};

vector<string> splitter(string str, char pattern);

void solver(string fileName);
