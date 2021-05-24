#include <iostream>
#include "structures.h"

class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	
	Node* root;

	void insertData(Spot*);
	void printTree(Node* N = nullptr);

private:
	int quadCapacity;

	void insert(Node*, Spot*);
	void insertFirstSpot(Spot*);
	void splitQuad(Node*);
	bool isSpotInArea(Spot*, Rectangle&);
	void defSubQuadsSize(Node*);
	void resizeTree(Node* );
	void expandRootQuad(Rectangle&, Spot*);
	void reinsertSpots(Node*);
	Node* chooseSubTree(Node*, Spot*);
};

