
#include "structures.h"
#include "QuadTree.h"
void search(double,double ,double ,string,string );
Point GetCentreOfRect(Rectangle);
bool IntersectCircleInRectangule(Rectangle,Circle);
bool CirclPointIntersect(Circle cir, Spot *point);
void PointsInCircleSearch(Node* , Point , double , string , string , vector< Spot* >& );

vector<Spot*> readSpotList(string);

int main(int argc, char** argv)
{
    Spot center;
    string type,subtype;
    center.latitude = atof(argv[2]);
    center.longitude = atof(argv[3]);
    double radius = atof(argv[4]);
    type = argv[5];
    subtype = argv[6];
    center.adapt();
    QuadTree tree;
    vector<Spot*> geoSpots;

    Spot cent;
    geoSpots = readSpotList("C:/Users/ramp8/Laboratorni_Z_OP/Laba5/data/spots.csv");
    for(Spot *i :geoSpots)
    {
        tree.insertData(i);
    }

    return 0;
}

vector<Spot*> readSpotList(string path){
    ifstream file(path);

    if (!file.is_open()){
        cout << "Failed to open spot list file!" << endl;
    }

    string buff;
    vector<Spot*> geoSpots;

    while (!file.eof()){
        Spot* newGeoSpot;

        getline(file, buff, ';');
        newGeoSpot->latitude = stof(buff);

        getline(file, buff, ';');
        newGeoSpot->longitude = stof(buff);

        getline(file, buff, ';');
        newGeoSpot->type = buff;

        getline(file, buff, ';');
        newGeoSpot->subtype = buff;

        getline(file, buff, ';');
        newGeoSpot->name = buff;

        getline(file, buff, ';');
        newGeoSpot->address = buff;

        getline(file, buff, '\n'); // Next spot

        geoSpots.push_back(newGeoSpot);
    }

    return geoSpots;
}

Point GetCentreOfRect(Rectangle rect)
{
    Point centre;
    centre.x = (rect.RT.x+rect.LB.x)/2;
    centre.y = (rect.RT.y+rect.LB.y)/2;
    return centre;
}

bool CirclPointIntersect(Circle cir, Spot *point) {
    return pow(cir.centre.x - point->x, 2) + pow(cir.centre.y - point->y, 2) <= pow(cir.rad, 2);
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

