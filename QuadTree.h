#include <iostream>
#include "structures.h"

class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	
	Node* root;

	void insertData(Spot*);

private:
	int quadCapacity;

	void insert(Node*, Spot*);
	void insertFirstSpot(Spot*);
	void splitQuad(Node*);
	bool isSpotInArea(Spot*, Rectangle&);
	Node* chooseSubTree(Node*, Spot*);
};

