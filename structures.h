#include <vector>
#include <string>
#include <math.h>
//using  namespace std;



struct Spot {
    double latitude;
    double longitude;
    std::string type;
    std::string subtype;
    std::string name;
    std::string address;
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
    Point() {}
    Point(double  x,double y)
    {
        this->x = x;
        this->y = y;
    }
};

struct Rect{
    Rect (){};
    Rect (Point a,Point b)
    {
        LB = a;
        RT = b;
    }

    std::vector<std::string> types;
    Point LB; // Left bottom corner
    Point RT; // Top right corner
};

struct Circle
{
    double rad;
    Point centre;
};

struct Node {
    std::vector<Node*> childs;
    std::vector<Spot*> data;
    Node* parent;
    Rect MBR;
    bool isLeaf;
};