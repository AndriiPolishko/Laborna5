#include "QuadTree.h"

QuadTree::QuadTree(double lon0, double lat0, double lon1, double lat1)
{
    quadCapacity = 10;

    root = new Node;

    root->isLeaf = true;

    Point LB;
    Point RT;

    LB.x = 6371.0 * cos(lat0 * 3.14159 / 180.) * (lon0 * 3.14159 / 180.);
    LB.y = (lat0 * 3.14159 / 180.) * 6371.0;
    RT.x = 6371.0 * cos(lat1 * 3.14159 / 180.) * (lon1 * 3.14159 / 180.);
    RT.y = (lat1 * 3.14159 / 180.) * 6371.0;

    root->MBR = Rect(LB, RT);
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
                if (isSpotInArea(newData, N->childs[i]->MBR))
                {

                    insert(newData, N->childs[i]);
                    bool flag;
                    for(auto i = 0;i<N->MBR.types.size();i++)
                    {
                       if( N->MBR.types[i] == newData->type ) flag = false;
                    }
                    if(flag)
                        N->MBR.types.push_back(newData->type);
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
            std::cout << N->data[i]->latitude << ';';
            std::cout << N->data[i]->longitude << ';';
            std::cout << N->data[i]->type << ';';
            std::cout << N->data[i]->subtype << ';';
            std::cout << N->data[i]->name << ';';
            std::cout << N->data[i]->address << ';' << std::endl;
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

bool QuadTree::isSpotInArea(Spot* data, Rect& quad)
{
    return (data->x >= quad.LB.x && data->x <= quad.RT.x &&
            data->y >= quad.LB.y && data->y <= quad.RT.y);

}