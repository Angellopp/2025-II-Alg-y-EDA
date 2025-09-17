#include <iostream>
#include "DemoVector.h"
#include "vector.h"
using namespace std;

void DemoVector(){
    CVector<int> v(0);
    for (int i = 1; i <= 100; ++i) v.insert(i);
    cout << "size = " << v.size() << endl;
    // Se muestra ultimos 5 para corroborrar
    for (size_t i = v.size() - 5; i < v.size(); ++i) cout << v[i] << ' ';
    cout << endl; // "... 96 97 98 99 100"
}
