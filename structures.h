#include <iostream>
#include <vector>
#include <string>

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
    Point LB; // Left bottom corner
    Point RT; // Top right corner
};

struct Circle
        {
    double rad;
    Point centre;
        };

struct Node {
    vector<Node*> childs;
    vector<Spot*> data;
    Node* parent;
    Rectangle MBR;
    bool isLeaf;
};