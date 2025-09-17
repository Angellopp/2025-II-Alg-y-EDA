#include <iostream>
#include "DemoVector.h"
#include "vector.h"
using namespace std;

void DemoVector(){
    CVector<int> v(0);
    for (int i = 1; i <= 5; ++i) v.insert(i);
    cout << "forward: ";
    for (auto it = v.begin(); it != v.end(); ++it) cout << *it << ' ';
    cout << "\nbackward: ";
    for (auto it = v.rbegin(); it != v.rend(); ++it) cout << *it << ' ';
    cout << endl; 
    // forward: 1 2 3 4 5 
    // backward: 5 4 3 2 1
}
