#include "functions.h"

vector<string> splitter(string str, char pattern) {
    vector<string> splitVector;
    stringstream ss(str); // Usar stringstream para facilitar la separacion
    string item;

    while (getline(ss, item, pattern)) {
        // Eliminar espacios en blanco al principio y al final de cada elemento
        item.erase(0, item.find_first_not_of(' ')); 
        item.erase(item.find_last_not_of(' ') + 1);

        // Agregar al vector solo si no esta vacio (evita agregar espacios vacios)
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

            int type = stoi(splitVector[0]);
            int M = stoi(splitVector[1]);
            int N = stoi(splitVector[2]);
            int T = stoi(splitVector[3]);

            if (type != MDVRP_TYPE) {
                throw runtime_error("Tipo de problema incorrecto. Se espera MDVRP (tipo 2).");
            }

            vector<DepositoDQ> depositosDQ(T);

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                
                // Verificar que hay suficientes valores en la linea
                if (splitVector.size() < 2) {
                    throw runtime_error("Formato de linea de deposito incorrecto.");
                }

                depositosDQ[i].duracionMaxima = stoi(splitVector[0]);
                depositosDQ[i].capacidadMaxima = stoi(splitVector[1]);
            }

            vector<Cliente> clientes(N);

            for (int i = 0; i < N; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');
                
                clientes[i].id = stoi(splitVector[0]);
                clientes[i].x = stof(splitVector[1]);
                clientes[i].y = stof(splitVector[2]);
                clientes[i].tiempoServicio = stoi(splitVector[3]);
                clientes[i].demanda = stoi(splitVector[4]);
                clientes[i].frecuencia = stoi(splitVector[5]);
                clientes[i].combinacionesVisita = stoi(splitVector[6]);
                
                for(int j = 7; j < splitVector.size(); j++) {
                    int valor = stoi(splitVector[j]);
                    clientes[i].ordenVisita.push_back(valor);
                }
            }

            vector<Deposito> depositos(T);

            for (int i = 0; i < T; i++) {
                getline(myFile, linea);
                splitVector = splitter(linea, ' ');

                depositos[i].id = stoi(splitVector[0]);
                depositos[i].x = stof(splitVector[1]);
                depositos[i].y = stof(splitVector[2]);
                depositos[i].tiempoServicio = stoi(splitVector[3]);
                depositos[i].demanda = stoi(splitVector[4]);
                depositos[i].frecuencia = stoi(splitVector[5]);
                depositos[i].combinacionesVisita = stoi(splitVector[6]);
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
