#include <iostream>
#include "DemoVector.h"
#include "vector.h"
using namespace std;

void DemoVector(){
    CVector<int> a(10);
    int x = 1, y = 2, z = 3;
    a.insert(x);
    a.insert(y);
    a.insert(z);

    CVector<int> b(std::move(a));
    cout << b << endl;                       // "1 2 3"
    cout << a.size() << endl;                // "0" (a quedo vacio)

}
