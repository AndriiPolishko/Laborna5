#include "structures.h"
#include <iostream>

class QuadTree
{
public:
    QuadTree(double, double, double, double);

    Node* root;

    void insert(Spot*, Node* N = nullptr);
    void print(Node* N = nullptr);

private:
    int quadCapacity;

    void split(Node*);
    bool isSpotInArea(Spot*, Rect&);
};