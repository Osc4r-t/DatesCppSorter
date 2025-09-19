#include "datesorter.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

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

//pequeña pausa
void pause(){
    cout << "Press enter to continue.";
    cin.ignore(); // limpia cualquier residuo
    cin.get();    // espera por el click de cualquier tecla
}

// this function loads movie data from a csv and adds them to the catalog
void loadOrderData(const string& filename, vector<Order*>& _orders) {
    ifstream file(filename);
    string line;

    while(getline(file, line)){
        stringstream ss(line);
        string s, min, hr, day, month, year, n, r, o, waste;

        // get each field from the csv line
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


        // convert strings to numbers
        int _s, _min, _hr, _day, _month, _n;
        _s = stoi(s);
        _min = stoi(min);
        _hr = stoi(hr);
        _day = stoi(day);
        _month = monthToInt(month);
        _n = stoi(n);

        // create a new movie and add to the catalog
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
    int M = L+(R-L)/2;

    if (L<R) {
        mergeSort(A,L,M,asc);
        mergeSort(A,M+1,R,asc);
        merge(A,L,M,R,asc);
    }
}

int main(){
    vector<Order*> orders;
    loadOrderData("orders.txt", orders);

    for (int i = 0; i < 10; i++){
        cout << *orders[i] <<endl;
    }
    cout << endl;
    int n = orders.size();
    mergeSort(orders, 0, n-1, true);

    for (int i = 0; i < 10; i++){
        cout << *orders[i] <<endl;
    }

    cout << (orders[1] == orders[2]) << endl;

    for (Order* order : orders) {
        delete order;
    }
    orders.clear();
}
