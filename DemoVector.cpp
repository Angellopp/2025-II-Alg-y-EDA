#include <iostream>
#include "DemoVector.h"
#include "vector.h"
using namespace std;

void DemoVector(){
    {
        CVector<int> tmp(10);
        int x = 1; 
        tmp.insert(x);
        tmp.insert(x);
        cout << tmp << endl; // "1"
        // aquí al salir del bloque se deberia llama al destructor ~CVector()
    }
    cout << "Fin de bloque (destructor invocado)" << endl;
}