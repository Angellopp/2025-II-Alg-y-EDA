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

    CVector<int> b(a); 
    b[0] = 99;              

    cout << a << endl;      //  "1 2 3"
    cout << b << endl;      //  "99 2 3"
}
