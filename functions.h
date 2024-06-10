#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

vector<string> splitter(string str, char pattern);

double calcularDistancia(double x1, double y1, double x2, double y2);

unsigned int generarSemillaAleatoria();

void solver(string fileName);
