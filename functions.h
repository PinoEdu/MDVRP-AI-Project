#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace std;

const int MDVRP_TYPE = 2;

struct Cliente {
    int id, demanda, tiempoServicio, frecuencia, combinacionesVisita;
    double x, y; 
    vector<int> ordenVisita;
};

struct Deposito {
    int duracionMaxima, capacidadMaxima;
};

vector<string> splitter(string str, char pattern);

void solver(string fileName);
