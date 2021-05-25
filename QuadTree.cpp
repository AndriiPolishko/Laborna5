#include "QuadTree.h"

QuadTree::QuadTree(double x0, double y0, double x1, double y1)
{
	root = nullptr;
	quadCapacity = 100;

	root = new Node;

	root->isLeaf = true;

	Point LB;
	Point RT;

	LB.x =  x0;
	LB.y = y0;
	RT.x = x1;
	RT.y = y1;

	cout << LB.x << " " << LB.y << endl;
	cout << RT.x << " " << RT.y << endl;

	root->MBR = Rectangle(LB, RT);
}

void QuadTree::insert(Spot* newData, Node* N)
{
	if (N == nullptr)
	{
		N = root;
	}
	
	if (N->isLeaf)
	{

		if (N->data.size() < quadCapacity)
		{
			N->data.push_back(newData);
		}
		else {
			split(N);
			for (int i = 0; i < 4; i++)
			{
				if (isSpotInArea(newData, N->childs[i]->MBR)) {
					insert(newData, N->childs[i]);
				}
			}
		}
	}
	else {
		for (int i = 0; i < 4; i++)
		{
			if (isSpotInArea(newData, N->childs[i]->MBR)) {
				insert(newData, N->childs[i]);
			}
		}
	}
}

void QuadTree::print(Node* N)
{
	if (N == nullptr)
	{
		N = root;
	}

	if (N->isLeaf)
	{
		for (int i = 0; i < N->data.size(); i++)
		{
			cout << N->data[i]->latitude << ';';
			cout << N->data[i]->longitude << ';';
			cout << N->data[i]->type << ';';
			cout << N->data[i]->subtype << ';';
			cout << N->data[i]->name << ';';
			cout << N->data[i]->address << ';' << endl;
		}
	}
	else {
		for (int i = 0; i < 4; i++)
		{
			print(N->childs[i]);
		}
	}
}

void QuadTree::split(Node* N)
{
	Node* LT = new Node;
	Node* RT = new Node;
	Node* RB = new Node;
	Node* LB = new Node;

	LT->isLeaf = true;
	RT->isLeaf = true;
	RB->isLeaf = true;
	LB->isLeaf = true;

	N->isLeaf = false;
	
	// LT init
	LT->MBR.LB.x = N->MBR.LB.x;
	LT->MBR.LB.y = (N->MBR.RT.y + N->MBR.LB.y) / 2;
	LT->MBR.RT.x = (N->MBR.RT.x + N->MBR.LB.x) / 2;
	LT->MBR.RT.y = N->MBR.RT.y;

	// RT init
	RT->MBR.LB.x = (N->MBR.RT.x + N->MBR.LB.x) / 2;
	RT->MBR.LB.y = (N->MBR.RT.y + N->MBR.LB.y) / 2;
	RT->MBR.RT.x = N->MBR.RT.x;
	RT->MBR.RT.y = N->MBR.RT.y;

	// RB init
	RB->MBR.LB.x = (N->MBR.RT.x + N->MBR.LB.x) / 2;
	RB->MBR.LB.y = N->MBR.LB.y;
	RB->MBR.RT.x = N->MBR.RT.x;
	RB->MBR.RT.y = (N->MBR.RT.y + N->MBR.LB.y) / 2;

	// LB init
	LB->MBR.LB.x = N->MBR.LB.x;
	LB->MBR.LB.y = N->MBR.LB.y;
	LB->MBR.RT.x = (N->MBR.RT.x + N->MBR.LB.x) / 2;
	LB->MBR.RT.y = (N->MBR.RT.y + N->MBR.LB.y) / 2;
	
	
	for (int i = 0; i < N->data.size(); i++)
	{
		if (isSpotInArea(N->data[i], LT->MBR))
		{
			LT->data.push_back(N->data[i]);
		}

		else if (isSpotInArea(N->data[i], RT->MBR))
		{
			RT->data.push_back(N->data[i]);
		}

		else if (isSpotInArea(N->data[i], RB->MBR))
		{
			RB->data.push_back(N->data[i]);
		}

		else if (isSpotInArea(N->data[i], LB->MBR))
		{
			LB->data.push_back(N->data[i]);
		}
	}

	N->childs = {LT, RT, RB, LB};
	N->data.clear();
}

bool QuadTree::isSpotInArea(Spot* data, Rectangle& quad)
{
	
	return (data->latitude >= quad.LB.x && data->latitude <= quad.RT.x &&
		data->longitude >= quad.LB.y && data->longitude <= quad.RT.y);
	
}
