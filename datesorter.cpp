#include "datesorter.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

//Convierte los strings de meses a int
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

void loadOrderData(const string& filename, vector<Order*>& _orders) {
    ifstream file(filename);//lee el archivo
    string line;

    while(getline(file, line)){
        stringstream ss(line);
        string s, min, hr, day, month, year, n, r, o, waste;

        // obtiene cada elemento de la orden
        getline(ss, month,' ');
        getline(ss, day, ' ');
        getline(ss, hr, ':');
        getline(ss, min, ':');
        getline(ss, s, ' ');
        getline(ss, waste, ':');//guarda las partes del formato que no nos interesan
        getline(ss, r, 'O');
        getline(ss, waste, ':');//guarda las partes del formato que no nos interesan
        getline(ss, o, '(');
        getline(ss, n, ')');


        // convierte strings a numeros
        int _s, _min, _hr, _day, _month, _n;
        _s = stoi(s);
        _min = stoi(min);
        _hr = stoi(hr);
        _day = stoi(day);
        _month = monthToInt(month);
        _n = stoi(n);

        // crea un nuevo elemento y lo agrega al vector
        _orders.push_back(new Order(_s,_min,_hr,_day,_month,_n,r,o));

    }

    file.close();
}

void merge(vector<Order*>& A, int L, int M, int R, bool asc = true){
    int s1 = M-L+1; //partition izquierda
    int s2 = R-M; //partition derecha

   vector<Order*> parteL(s1), parteR(s2);

    int i,j,k;

    for (i=0;i<s1;i++){
        //Copia los elementos de la primera mitad
        parteL[i] = A[L+i];
    }
    for (j=0;j<s2;j++){
        //Copia los elementos de la segunda mitad
        parteR[j] = A[M+1+j];
    }

    i=0;
    j=0;
    k=L;

    while(i<s1 && j<s2){
        // Compara los elementos de las dos mitades y los combina en orden ascendente
        if (asc ? (*parteL[i]<=*parteR[j]) : (*parteL[i] >=*parteR[j])){
            A[k] = parteL[i];
            i++;
        }else{
            A[k] = parteR[j];
            j++;
        }
        k++;
    }

    //Copia los elementos restantes de la primera mitad
    while(i<s1){
        A[k] = parteL[i];
        i++;
        k++;
    }
    //Copia los elementos restantes de la segunda mitad
    while(j<s2){
        A[k] = parteR[j];
        j++;
        k++;
    }
}
void mergeSort(vector<Order*>& A, int L, int R, bool asc = true) {
    int M = L+(R-L)/2;//crea el indice medio

    if (L<R) {
        mergeSort(A,L,M,asc);//particion izquiera
        mergeSort(A,M+1,R,asc);//particion derecha
        merge(A,L,M,R,asc);//una las partes
    }
}

int binarySearch(vector<Order*>& A, int n, Order x){
    int bajo = 0, alto = n-1;
    int medio = 0;
    while (bajo <= alto){
        medio = (bajo + alto)/2; //Calcula el punto medio del arreglo

        if (*A[medio] == x){ //Verifica si se ha encontrado el valor buscado
            return medio;
        }else if(*A[medio] < x){//Si el valor buscado es mayor que el valor del punto medio se busca en la parte superior del arreglo
            bajo = medio + 1;
        }else{//Si el valor buscado es menor que el valor del punto medio se busca en la parte inferior del arreglo
            alto = medio - 1;
        }
    }

    return (bajo >= alto) ? bajo : alto;//Devuelve  el indice que corresponda al primer valor mayor que el deseado
}

int main(){

    vector<Order*> orders;//crea el vector donde se guardaran los objetos
    loadOrderData("orders.txt", orders);//lee el archivo de texto y guarda los objetos
    int n = orders.size();

    //acomoda los objetos
    mergeSort(orders, 0, n-1, true);

    //Crear un archivo de texto con las ordenes en orden
    ofstream archivo("ordenado.txt");
    for (int i = 0; i < n; i++) {
        archivo << *orders[i] <<endl;
    }
    archivo.close();

    //muestra los primeros diez objetos en orden ascendente
    for (int i = 0; i < 10; i++){
        cout << *orders[i] <<endl;
    }

    //Le pregunta al usuario el rango que desea buscar
    int m,d,h,minute,s;
    //obtiene la fecha inical
    cout << "Que rango de fechas desea buscar?" << endl << "Ingrese fecha inicial (M D H Min S): ";
    cin >> m >> d >> h >> minute >> s;
    Order x = Order(s,minute,h,d,m);
    int rango_bajo = binarySearch(orders, n, x);

    //obtiene la fecha final
    cout << "Ingrese fecha final (M D H Min S): ";
    cin >> m >> d >> h >> minute >> s;
    x = Order(s,minute,h,d,m);
    int rango_alto = binarySearch(orders, n, x);

    //muestra los valores en el rango deseado
    for (int i = rango_bajo; i < rango_alto; i++) {
        cout << *orders[i] <<endl;
    }

    bool guardar;
    cout << endl << "Deseas guardas los datos (1/0)? "; //pide al usuario si quiere guardar los datos
    cin >> guardar;
    if (guardar) {
        //guarda los datos en un txt
        ofstream archivo2("busqueda.txt");
        for (int i = rango_bajo; i < rango_alto; i++) {
            archivo2 << *orders[i] <<endl;
        }
        archivo2.close();
    }
    
    //borra los pointers
    for (Order* order : orders) {
        delete order;
    }
    orders.clear();

    return 0;
}
