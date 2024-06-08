#include "functions.h"

vector<string> splitter(string str, char pattern) {
    int posInt = 0;
    int posFound = 0;
    string splitted;
    vector<string> splitVector;

    while (posFound >= 0) {
        posFound = str.find(pattern, posInt);
        splitted = str.substr(posInt, posFound - posInt);
        posInt = posFound + 1;
        splitVector.push_back(splitted);
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

            int type = stoi(splitVector[0]);
            int M = stoi(splitVector[1]);
            int N = stoi(splitVector[2]);
            int T = stoi(splitVector[3]);

            if (type != MDVRP_TYPE) {
                throw runtime_error("Tipo de problema incorrecto. Se espera MDVRP (tipo 2).");
            }

            vector<Deposito> depositos(T);

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                
                // Verificar que hay suficientes valores en la linea
                if (splitVector.size() < 2) {
                    throw runtime_error("Formato de linea de deposito incorrecto.");
                }

                depositos[i].duracionMaxima = stoi(splitVector[0]);
                depositos[i].capacidadMaxima = stoi(splitVector[1]);
            }

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
