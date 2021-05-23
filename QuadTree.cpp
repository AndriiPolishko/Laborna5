#include "QuadTree.h"

QuadTree::QuadTree()
{
	root = nullptr;
	quadCapacity = 100;
}

QuadTree::~QuadTree()
{
}

void QuadTree::insertData(Spot* newData)
{
	if (!root)
	{
		insertFirstSpot(newData);
	}
	else {
		insert(root, newData);
	}
}

void QuadTree::insert(Node* N, Spot* newData)
{
	if (N->isLeaf)
	{
		if (N->data.size() < quadCapacity)
		{
			N->data.push_back(newData);
		}
		else {
			splitQuad(N);
			Node* childNode = chooseSubTree(N, newData);
			insert(childNode, newData);
		}
	}
	else {
		Node* childNode = chooseSubTree(N, newData);
		insert(childNode, newData);
	}
}

void QuadTree::insertFirstSpot(Spot* newData)
{
	root = new Node;

	root->data.push_back(newData);

	root->isLeaf = true;

	root->MBR.LB = { newData->x, newData->y };
	root->MBR.RT = { newData->x, newData->y };
}

void QuadTree::splitQuad(Node* N)
{
	N->isLeaf = false;

	Rectangle PR = N->MBR; // PR - for Parent Rectangle

	Node* LT = new Node;
	Node* RT = new Node;
	Node* RB = new Node;
	Node* LB = new Node;

	LT->isLeaf = true;
	RT->isLeaf = true;
	RB->isLeaf = true;
	LB->isLeaf = true;

	// LT MBR init
	LT->MBR.LB.x = PR.LB.x;
	LT->MBR.LB.y = (PR.LB.y + PR.RT.y) / 2;
	LT->MBR.RT.x = (PR.LB.x + PR.RT.x) / 2;
	LT->MBR.RT.y = PR.RT.y;

	// RT MBR init
	RT->MBR.LB.x = (PR.LB.x + PR.RT.x) / 2;
	RT->MBR.LB.y = (PR.LB.y + PR.RT.y) / 2;
	RT->MBR.RT.x = PR.RT.x;
	RT->MBR.RT.y = PR.RT.y;

	// RB MBR init
	RB->MBR.LB.x = PR.LB.x;
	RB->MBR.LB.y = PR.LB.y;
	RB->MBR.RT.x = (PR.LB.x + PR.RT.x) / 2;
	RB->MBR.RT.y = (PR.LB.y + PR.RT.y) / 2;

	// LT MBR init
	LB->MBR.LB.x = PR.LB.x;
	LB->MBR.LB.y = PR.LB.y;
	LB->MBR.RT.x = (PR.LB.x + PR.RT.x) / 2;
	LB->MBR.RT.y = (PR.LB.y + PR.RT.y) / 2;
}

bool QuadTree::isSpotInArea(Spot* newData, Rectangle& quad)
{
	if (newData->x <= quad.RT.x && newData->x >= quad.LB.x &&
		newData->y <= quad.RT.y && newData->y >= quad.LB.y)
		return true;

	return false;
}

Node* QuadTree::chooseSubTree(Node* N, Spot* newData)
{
	for (int i = 0; i < 4; i++)
	{
		if (isSpotInArea(newData, N->childs[i]->MBR))
			return N->childs[i];
	}
}
