#include <iostream>
#include <vector>
#include <string>
#include <math.h>

using  namespace std;

struct Spot {
    double latitude;
    double longitude;
    string type;
    string subtype;
    string name;
    string address;
    double x,y;
    void adapt() {
        //converting into xOy coordinates
        x = 6371.0 * cos(latitude * 3.14159 / 180.) * (longitude * 3.14159 / 180.);
        y = (latitude * 3.14159 / 180.) * 6371.0;

        
    }
};

struct Point{
    double x;
    double y;
    Point()
    {}
    Point(double  x,double y)
    {
        this->x = x;
        this->y = y;
    }
};

struct Rectangle{
public:
    Rectangle(){};
    Point LB; // Left bottom corner
    Point RT; // Top right corner
    Rectangle(Point point, Point point1)
    {
    LB = point;
    RT = point1;
    }

};

struct Circle
        {
    double rad;
    Spot centre;
        };

struct Node {
    vector<Node*> childs;
    vector<Spot*> data;
    Node* parent;
    Rectangle MBR;
    bool isLeaf;
};


