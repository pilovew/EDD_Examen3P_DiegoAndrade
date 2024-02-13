#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

const int sizeDatos = 100;
const int sizeColision = sizeDatos / 10;
const int maxOcupacion = 80; 

struct Nodo {
    char code[4] = "";
};

Nodo dataArea[sizeDatos];
Nodo collisionArea[sizeColision];
int dataCount = 0;
int collisionCount = 0;

int funcionHash(const char* codigo);
void cargarDatosDesdeArchivo(const string& nombreArchivo);
void agregarDatosDesdeTeclado();
void eliminarDatos(const char* codigo);
void buscarDatos(const char* codigo);
void mostrarAreaDatos();
void mostrarAreaColisiones();
void guardarDatosEnArchivo(const string& nombreArchivo);
void cargarDatosDesdeArchivo(const string& nombreArchivo);
void mostrarOcupacionColisiones();
void insertarEnAreaColisiones(const Nodo& nuevoRegistro);
void mostrarOcupacionDatos();

int main() {
    int opcion = 0;
    string nombreArchivo;

    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Cargar datos desde archivo" << endl;
        cout << "2. Agregar datos desde teclado" << endl;
        cout << "3. Eliminar datos" << endl;
        cout << "4. Buscar datos" << endl;
        cout << "5. Mostrar area de datos" << endl;
        cout << "6. Mostrar area de colisiones" << endl;
        cout << "7. Guardar datos en archivo" << endl;
        cout << "8. Cargar datos desde archivo de datos" << endl;
        cout << "9. Mostrar ocupacion de area de colisiones" << endl;
        cout << "10. Mostrar ocupacion de area de datos" << endl;
        cout << "0. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            cout << "Introduce el nombre del archivo para cargar: ";
            cin >> nombreArchivo;
            cargarDatosDesdeArchivo(nombreArchivo);
            break;
        case 2:
            agregarDatosDesdeTeclado();
            break;
        case 3: {
            char codigo[4];
            cout << "Introduce el codigo del dato a eliminar: ";
            cin >> codigo;
            eliminarDatos(codigo);
            break;
        }
        case 4: {
            char codigo[4];
            cout << "Introduce el codigo del dato a buscar: ";
            cin >> codigo;
            buscarDatos(codigo);
            break;
        }
        case 5:
            mostrarAreaDatos();
            break;
        case 6:
            mostrarAreaColisiones();
            break;
        case 7:
            cout << "Introduce el nombre del archivo para guardar: ";
            cin >> nombreArchivo;
            guardarDatosEnArchivo(nombreArchivo);
            break;
        case 8:
            cout << "Introduce el nombre del archivo de datos para cargar: ";
            cin >> nombreArchivo;
            cargarDatosDesdeArchivo(nombreArchivo);
            break;
        case 9:
            mostrarOcupacionColisiones();
            break;
        case 10:
            mostrarOcupacionDatos();
            break;
        case 0:
            return 0;
        default:
            cout << "Opcion no valida. Por favor, intenta de nuevo." << endl;
            break;
        }
    }

    return 0;
}


int funcionHash(const char* codigo) {
    return (codigo[0] - 'D' + (codigo[1] - 'D') * 26 + (codigo[2] - 'D') * 26 * 26) % sizeDatos;
}

void cargarDatosDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string codigo;
        while (getline(ss, codigo, '\t')) { // Usar tabulación como delimitador
            if (codigo.length() >= 3) { // Asegurar que el código tenga al menos 3 caracteres
                Nodo nuevoRegistro;
                strncpy_s(nuevoRegistro.code, codigo.c_str(), 3);
                nuevoRegistro.code[3] = '\0'; // Asegurar terminación

                int indice = funcionHash(nuevoRegistro.code);

                if (dataArea[indice].code[0] == '\0') { // Espacio disponible
                    dataArea[indice] = nuevoRegistro;
                    dataCount++; // Incrementar el contador de elementos en el área de datos
                }
                else {
                    cout << "Colisión detectada para el índice: " << indice << ". Intentando insertar en área de colisiones." << endl;
                    insertarEnAreaColisiones(nuevoRegistro);
                    // Considera si debes incrementar dataCount aquí dependiendo de cómo quieras contar las colisiones
                }
            }
            else {
                cout << "Código inválido o demasiado corto: " << codigo << endl;
            }
        }
    }
    archivo.close();
}


void agregarDatosDesdeTeclado() {
    Nodo nuevoRegistro;
    cout << "Introduce el codigo de 3 letras: ";
    cin >> nuevoRegistro.code;
    if (strlen(nuevoRegistro.code) != 3) {
        cout << "Codigo inválido. Debe tener 3 letras." << endl;
        return;
    }

    int indice = funcionHash(nuevoRegistro.code);
    if (dataArea[indice].code[0] == '\0') {
        dataArea[indice] = nuevoRegistro;
        dataCount++;
        cout << "Registro añadido en el area de datos." << endl;
    }
    else {
        insertarEnAreaColisiones(nuevoRegistro);
    }
    mostrarOcupacionDatos();
}

void buscarDatos(const char* code) {
    int index = funcionHash(code);
    if (strcmp(dataArea[index].code, code) == 0) {
        cout << "Registro encontrado en el area de datos: " << dataArea[index].code << endl;
    }
    else {
        // Buscar en el área de colisiones
        for (int i = 0; i < sizeColision; i++) {
            if (strcmp(collisionArea[i].code, code) == 0) {
                cout << "Registro encontrado en el area de colisiones: " << collisionArea[i].code << endl;
                return;
            }
        }
        cout << "Registro no encontrado." << endl;
    }
}


void guardarDatosEnArchivo(const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    for (int i = 0; i < sizeDatos; i++) {
        if (dataArea[i].code[0] != '\0') { // Si el registro no está vacío
            archivo << dataArea[i].code << "\n";
        }
    }
    for (int i = 0; i < sizeColision; i++) {
        if (collisionArea[i].code[0] != '\0') {
            archivo << collisionArea[i].code << "\n";
        }
    }
    archivo.close();
}

void eliminarDatos(const char* codigo) {
    int indice = funcionHash(codigo);
    if (strcmp(dataArea[indice].code, codigo) == 0) {
        dataArea[indice].code[0] = '\0'; // Marcar como eliminado
        cout << "Registro eliminado del area de datos." << endl;
        dataCount--;
    }
    else {
        // Buscar y eliminar en el área de colisiones si es necesario
        bool encontrado = false;
        for (int i = 0; i < sizeColision && !encontrado; i++) {
            if (strcmp(collisionArea[i].code, codigo) == 0) {
                collisionArea[i].code[0] = '\0'; // Marcar como eliminado
                cout << "Registro eliminado del area de colisiones." << endl;
                collisionCount--;
                encontrado = true;
            }
        }
        if (!encontrado) {
            cout << "Registro no encontrado." << endl;
        }
    }
}


void insertarEnAreaColisiones(const Nodo& nuevoRegistro) {
    for (int i = sizeColision - 1; i >= 0; i--) {
        if (collisionArea[i].code[0] == '\0') {
            collisionArea[i] = nuevoRegistro;
            collisionCount++;
            cout << "Registro añadido en el area de colisiones." << endl;
            return;
        }
    }
    cout << "Area de colisiones llena. No se puede añadir el registro." << endl;
}

void mostrarAreaDatos() {
    cout << "Área de Datos:" << endl;
    for (int i = 0; i < sizeDatos; ++i) {
        if (dataArea[i].code[0] != '\0') { // Verifica que el código no esté vacío
            cout << "Índice: " << i << " - Código: " << dataArea[i].code << endl;
        }
    }
}

void mostrarAreaColisiones() {
    cout << "Área de Colisiones:" << endl;
    for (int i = 0; i < sizeColision; ++i) {
        if (collisionArea[i].code[0] != '\0') { // Verifica que el código no esté vacío
            cout << "Índice: " << i << " - Código: " << collisionArea[i].code << endl;
        }
    }
}

void mostrarOcupacionDatos() {
    cout << "Ocupacion del area de datos: " << (dataCount * 100 / sizeDatos) << "%" << endl;
}

void mostrarOcupacionColisiones() {
    cout << "Ocupacion del area de colisiones: " << ((collisionCount * 100 / sizeColision)/10) << "%" << endl;
}
