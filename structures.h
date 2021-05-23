#include <iostream>
#include "math.h"
using  namespace std;
using  namespace std;
struct Spot {
    float latitude;
    float longitude;
    string type;
    string subtype;
    string name;
    string address;
    double x,y;
    void adapt() {
        //converting into xOy coordinates
        x = 6371 * cos(latitude * 3.14159 / 180.) * (longitude * 3.14159 / 180.);
        y = (latitude * 3.14159 / 180.) * 6371;
    }
};
