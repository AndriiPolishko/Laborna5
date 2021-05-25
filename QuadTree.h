#include <iostream>
#include "structures.h"

class QuadTree
{
public:
	QuadTree(double, double, double, double);

	Node* root;

	void insert(Spot*, Node* N = nullptr);
	void print(Node* N = nullptr);


    double dist(Spot , Spot );
    bool CirclPointIntersect(Circle cir, Spot point);
    Point GetCentreOfRect(Rectangle);
    bool IntersectCircleInRectangule(Rectangle,Circle);
    bool isDotInCirc(Circle,Spot *);
    void search(Node * ,double r,double lon,double lat,string type,vector<Spot*> & res);


private:
	int quadCapacity;
	
	void split(Node*);
	bool isSpotInArea(Spot*, Rectangle&);
};

