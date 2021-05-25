#include <fstream>
#include "QuadTree.h"
#include <math.h>
vector<Spot> readSpotList(string);
double tod(string);

bool IntersectCircleInRectangule(Rectangle,Circle);
bool CirclPointIntersect(Circle cir, Spot *point);
Point GetCentreOfRect(Rectangle);
double dist(Spot , Spot * );

void PointsInCircleSearch(Node* , Spot , double , string , string , vector< Spot* >& );

int main(int argc, char** argv)
{
    QuadTree tree(22, 44, 41, 52);
    vector<Spot> geoSpots;

    Spot cent;
    cent.longitude = atof(argv[3]);
    cent.latitude = atof(argv[2]);
    double rad = atof(argv[4]);
    cent.adapt();
    geoSpots = readSpotList("spots.csv");

    cout << geoSpots.size() << endl;
    for(int i = 0; i < geoSpots.size(); i++)
    {
        tree.insert(&geoSpots[i]);
    }

    tree.print();
    string type = argv[5],subtype = argv[6];


    double currRadius = 1.;
    vector<Spot*> res;
    while ( currRadius <= (1<<13) ) {
        PointsInCircleSearch(tree.root, cent, currRadius, type, subtype, res);
        if (res.size() > 0)break;
        currRadius *= 2;
    }
    double minDist = 1e9;
    int pos = -1;
    for (int i = 0; i < res.size(); i++) {
        if ( dist(cent, res[i]) < minDist) {
            minDist = dist(cent, res[i]);
            pos = i;
        }
    }
    if (pos != -1) {
        cout << res[pos] << endl;
    }
    else {
        cout << "Not found!\n";
    }

    return 0;
}

vector<Spot> readSpotList(string path){
    ifstream file(path);

    if (!file.is_open()){
        cout << "Failed to open spot list file!" << endl;
    }

    string buff;
    vector<Spot> geoSpots;

    while (!file.eof()){
        Spot newGeoSpot;

        getline(file, buff, ';');
        newGeoSpot.latitude = tod(buff);

        getline(file, buff, ';');
        newGeoSpot.longitude = tod(buff);

        getline(file, buff, ';');
        newGeoSpot.type = buff;

        getline(file, buff, ';');
        newGeoSpot.subtype = buff;

        getline(file, buff, ';');
        newGeoSpot.name = buff;

        getline(file, buff, ';');
        newGeoSpot.address = buff;

        getline(file, buff, '\n'); // Next spot

        newGeoSpot.adapt();

        geoSpots.push_back(newGeoSpot);
    }

    return geoSpots;
}

double tod(string str) {
    str[str.find(',')] = '.';

    return stod(str);
}

bool CirclPointIntersect(Circle cir, Spot *point) {
    return pow(cir.centre.x - point->x, 2) + pow(cir.centre.y - point->y, 2) <= pow(cir.rad, 2);
}

Point GetCentreOfRect(Rectangle rect)
{
    Point centre;
    centre.x = (rect.RT.x+rect.LB.x)/2;
    centre.y = (rect.RT.y+rect.LB.y)/2;
    return centre;
}

bool IntersectCircleInRectangule(Rectangle rec,Circle cir)
{
    Point centre = GetCentreOfRect(rec);
    if(centre.x == cir.centre.x && centre.y == cir.centre.y)
        return true;
    double  xn = max(rec.LB.x,min(cir.centre.x,rec.RT.x)),
            yn = max(rec.LB.y,min(cir.centre.y,rec.RT.y));
    double dx = xn - cir.centre.x,dy = yn - cir.centre.y;
    return (dx*dx + dy*dy)<(cir.rad*cir.rad);
}

void PointsInCircleSearch(Node* node, Point centre, double R, string type, string subtype, vector< Spot* >& result) {
    Circle cir;
    cir.rad = R,cir.centre = centre;
    if (node->childs.size() > 0) {
        if (IntersectCircleInRectangule(node->MBR,cir)) {
            for (int i = 0; i < node->childs.size(); i++) {
                PointsInCircleSearch(node->childs[i], centre, R, type, subtype, result);
            }
        }
    }
    else
        for(int i =0;i<node->data.size();i++)
        {
            if (CirclPointIntersect(cir, node->data[i]) && node->data[i]->type == type && node->data[i]->subtype == subtype) {
                result.push_back(node->data[i]);
            }
        }

}

double dist(Spot A, Spot * B) {
    return sqrt( pow(A.x - B->x, 2) + pow(A.y - B->y, 2) );
}