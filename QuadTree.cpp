#include "QuadTree.h"

QuadTree::QuadTree()
{
	root = nullptr;
	quadCapacity = 200;
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

void QuadTree::printTree(Node* N)
{
	if (N == nullptr)
	{
		N = root;
	}
	if (N->isLeaf)
	{
		for (int i = 0; i < N->data.size(); i++)
		{
			cout << N->data[i]->latitude<< "; ";
			cout << N->data[i]->longitude << "; ";
			cout << N->data[i]->type << "; ";
			cout << N->data[i]->subtype << "; ";
			cout << N->data[i]->name << "; ";
			cout << N->data[i]->address << "; " << endl;
		}
	}
	else {
		for (int i = 0; i < N->childs.size(); i++)
		{
			printTree(N->childs[i]);
		}
	}
}

void QuadTree::insert(Node* N, Spot* newData)
{
	if (!isSpotInArea(newData, root->MBR))
	{
		expandRootQuad(root->MBR, newData);
		resizeTree(root);
		reinsertSpots(root);

	}
	if (N->isLeaf)
	{
		if (N->data.size() < quadCapacity)
		{
			N->data.push_back(newData);
		}
		else {
			splitQuad(N);
			Node* childNode = new Node;
			childNode = chooseSubTree(N, newData);
			insert(childNode, newData);
		}
	}
	else {
		Node* childNode = new Node; 
		childNode = chooseSubTree(N, newData);
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

	Node* LT = new Node;
	Node* RT = new Node;
	Node* RB = new Node;
	Node* LB = new Node;

	LT->isLeaf = true;
	RT->isLeaf = true;
	RB->isLeaf = true;
	LB->isLeaf = true;

	// Tie them to their parent rectangle
	N->childs = { LT, RT, RB, LB };

	defSubQuadsSize(N);
}

bool QuadTree::isSpotInArea(Spot* newData, Rectangle& quad)
{
	if (newData->x <= quad.RT.x && newData->x > quad.LB.x &&
		newData->y <= quad.RT.y && newData->y > quad.LB.y)
		return true;

	return false;
}

void QuadTree::defSubQuadsSize(Node* N)
{
	// LT MBR init
	N->childs[0]->MBR.LB.x = N->MBR.LB.x;
	N->childs[0]->MBR.LB.y = (N->MBR.LB.y + N->MBR.RT.y) / 2;
	N->childs[0]->MBR.RT.x = (N->MBR.LB.x + N->MBR.RT.x) / 2;
	N->childs[0]->MBR.RT.y = N->MBR.RT.y;

	// RT MBR init
	N->childs[1]->MBR.LB.x = (N->MBR.LB.x + N->MBR.RT.x) / 2;
	N->childs[1]->MBR.LB.y = (N->MBR.LB.y + N->MBR.RT.y) / 2;
	N->childs[1]->MBR.RT.x = N->MBR.RT.x;
	N->childs[1]->MBR.RT.y = N->MBR.RT.y;

	// RB MBR init
	N->childs[2]->MBR.LB.x = N->MBR.LB.x;
	N->childs[2]->MBR.LB.y = N->MBR.LB.y;
	N->childs[2]->MBR.RT.x = (N->MBR.LB.x + N->MBR.RT.x) / 2;
	N->childs[2]->MBR.RT.y = (N->MBR.LB.y + N->MBR.RT.y) / 2;

	// LT MBR init
	N->childs[3]->MBR.LB.x = N->MBR.LB.x;
	N->childs[3]->MBR.LB.y = N->MBR.LB.y;
	N->childs[3]->MBR.RT.x = (N->MBR.LB.x + N->MBR.RT.x) / 2;
	N->childs[3]->MBR.RT.y = (N->MBR.LB.y + N->MBR.RT.y) / 2;

}

void QuadTree::resizeTree(Node* N)
{
	if (!N->isLeaf)
	{
		defSubQuadsSize(N);

		for (int i = 0; i < 4; i++)
			resizeTree(N->childs[i]);
	}
}

void QuadTree::expandRootQuad(Rectangle& oldMBR, Spot* newData)
{
	cout << ">";
	Point newSpot = { newData->latitude, newData->longitude };

	float maxX, maxY, minX, minY;

	maxX = max(oldMBR.RT.x, newSpot.x);
	maxY = max(oldMBR.RT.y, newSpot.y);

	minX = min(oldMBR.LB.x, newSpot.x);
	minY = min(oldMBR.LB.y, newSpot.y);

	oldMBR = {
		Point(minX, minY),
		Point(maxX, maxY)
	};
}

void QuadTree::reinsertSpots(Node* N)
{
	if (N->isLeaf)
	{
		for (int i = 0; i < N->data.size(); i++)
		{
			if (!isSpotInArea(N->data[i], N->MBR)) {
				Spot* dataUnit = new Spot;
				dataUnit = N->data[i];
				N->data.erase(N->data.begin() + i);
				insertData(dataUnit);
			}
		}
	}
	else {
		for (int i = 0; i < N->childs.size(); i++)
		{
			reinsertSpots(N->childs[i]);
		}
	}
}

Node* QuadTree::chooseSubTree(Node* N, Spot* newData)
{
	for (int i = 0; i < 4; i++)
	{
		if (isSpotInArea(newData, N->childs[i]->MBR)) {
			return N->childs[i];
		}
	}
}
