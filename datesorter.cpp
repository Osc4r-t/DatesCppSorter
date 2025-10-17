#include "datesorter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//Convierte los strings de meses a int (igual que tu versión)
int monthToInt(string m){
    if (m == "ene"){
        return 1;
    } else if (m == "Feb"){
        return 2;
    } else if (m == "Mar"){
        return 3;
    } else if (m == "Abr"){
        return 4;
    } else if (m == "May"){
        return 5;
    } else if (m == "Jun"){
        return 6;
    } else if (m == "Jul"){
        return 7;
    } else if (m == "Ago"){
        return 8;
    } else if (m == "Sep"){
        return 9;
    } else if (m == "Oct"){
        return 10;
    } else if (m == "Nov"){
        return 11;
    } else if (m == "Dic"){
        return 12;
    }
    return 0;
}

//pequeña pausa (igual que tu versión)
void pause(){
    cout << "Press enter to continue.";
    cin.ignore();
    cin.get();
}

// Carga datos a un ARREGLO de punteros: out[i] = new Order(...)
// - capacity: tamaño máximo del arreglo
// - loaded (salida): cuántos elementos se cargaron realmente
void loadOrderData(const string& filename, Order** out, int capacity, int &loaded) {
    ifstream file(filename);
    string line;
    int i = 0;

    while (i < capacity && getline(file, line)){
        stringstream ss(line);
        string s, min, hr, day, month, year, n, r, o, waste;

        // obtiene cada elemento de la orden (tu mismo parseo)
        getline(ss, month,' ');
        getline(ss, day, ' ');
        getline(ss, hr, ':');
        getline(ss, min, ':');
        getline(ss, s, ' ');
        getline(ss, waste, ':');
        getline(ss, r, 'O');
        getline(ss, waste, ':');
        getline(ss, o, '(');
        getline(ss, n, ')');

        // convierte strings a numeros
        int _s   = stoi(s);
        int _min = stoi(min);
        int _hr  = stoi(hr);
        int _day = stoi(day);
        int _month = monthToInt(month);
        int _n   = stoi(n);

        // crea y guarda puntero
        out[i] = new Order(_s,_min,_hr,_day,_month,_n,r,o);
        ++i;
    }
    file.close();
    loaded = i;

    // (opcional) inicializa el resto a nullptr para seguridad
    for (int j = loaded; j < capacity; ++j) out[j] = nullptr;
}

// ---------- MERGE SORT sobre arreglo de punteros (Order**) ----------
void merge(Order** A, int L, int M, int R, bool asc = true){
    int s1 = M - L + 1;  // tamaño izquierda
    int s2 = R - M;      // tamaño derecha

    // temporales (arreglos de punteros)
    Order** parteL = new Order*[s1];
    Order** parteR = new Order*[s2];

    // copiar mitades
    for (int i = 0; i < s1; ++i) parteL[i] = A[L + i];
    for (int j = 0; j < s2; ++j) parteR[j] = A[M + 1 + j];

    int i = 0, j = 0, k = L;

    // fusionar comparando los objetos apuntados
    while (i < s1 && j < s2){
        bool takeLeft = asc ? (*parteL[i] <= *parteR[j])
                            : (*parteL[i] >= *parteR[j]);
        if (takeLeft) {
            A[k++] = parteL[i++];
        } else {
            A[k++] = parteR[j++];
        }
    }
    while (i < s1) A[k++] = parteL[i++];
    while (j < s2) A[k++] = parteR[j++];

    delete[] parteL;
    delete[] parteR;
}

void mergeSort(Order** A, int L, int R, bool asc = true) {
    if (L >= R) return;
    int M = L + (R - L) / 2;
    mergeSort(A, L,     M, asc);
    mergeSort(A, M + 1, R, asc);
    merge(A, L, M, R, asc);
}

// Búsqueda binaria estilo lower_bound en arreglo de punteros
// Devuelve el primer índice donde *A[idx] >= x (si existe), útil para rangos
int binarySearch(Order** A, int n, const Order& x){
    int lo = 0, hi = n;           // intervalo [lo, hi)
    while (lo < hi){
        int mid = lo + (hi - lo) / 2;
        if (*A[mid] < x) lo = mid + 1;
        else             hi = mid;
    }
    return lo; // posición de inserción (o primer >= x)
}

int main(){

    const int n = 10000;       // capacidad
    Order* _orders[n];         // arreglo de PUNTEROS a Order
    int loaded = 0;            // cuántos cargamos

    loadOrderData("orders.txt", _orders, n, loaded);

    // muestra los primeros 10 (o menos si loaded < 10)
    // int limit = (loaded < 10) ? loaded : 10;
    // for (int i = 0; i < limit; ++i){
    //     cout << *_orders[i] << '\n';
    // }

    // ordena si hay datos
    if (loaded > 0) {
        mergeSort(_orders, 0, loaded - 1, true);
    }

    // Crear un archivo con las ordenes en orden
    ofstream archivo("ordenado.txt");
    if (archivo){
        for (int i = 0; i < loaded; ++i) {
            archivo << *_orders[i] << '\n';
        }
        archivo.close();
    }

    // muestra los primeros diez objetos en orden ascendente
    // cout << "\n--- Primeros 10 ordenados ---\n";
    // limit = (loaded < 10) ? loaded : 10;
    // for (int i = 0; i < limit; ++i){
    //     cout << *_orders[i] << '\n';
    // }

    // Le pregunta al usuario el rango que desea buscar
    int m,d,h,minute,s;
    cout << "\n¿Qué rango de fechas desea buscar?\nIngrese fecha inicial (M D H Min S): ";
    cin >> m >> d >> h >> minute >> s;
    Order x1 = Order(s,minute,h,d,m);
    int rango_bajo = binarySearch(_orders, loaded, x1);

    cout << "Ingrese fecha final (M D H Min S): ";
    cin >> m >> d >> h >> minute >> s;
    Order x2 = Order(s,minute,h,d,m);
    int rango_alto = binarySearch(_orders, loaded, x2);

    if (rango_bajo > rango_alto) {
        int tmp = rango_bajo; rango_bajo = rango_alto; rango_alto = tmp;
    }
    if (rango_bajo < 0) rango_bajo = 0;
    if (rango_alto > loaded) rango_alto = loaded;

    cout << "\n--- Resultados en rango ---\n";
    for (int i = rango_bajo; i < rango_alto; ++i) {
        cout << *_orders[i] << '\n';
    }

    bool guardar;
    cout << "\n¿Deseas guardas los datos (1/0)? ";
    cin >> guardar;
    if (guardar) {
        ofstream archivo2("busqueda.txt");
        if (archivo2){
            for (int i = rango_bajo; i < rango_alto; ++i) {
                archivo2 << *_orders[i] << '\n';
            }
            archivo2.close();
        }
    }

    // borra los objetos creados 
    for (int i = 0; i < loaded; ++i) {
        delete _orders[i];
        _orders[i] = nullptr;
    }

    return 0;
}
