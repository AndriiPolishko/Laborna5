#include <fstream>
#include <iomanip>
#include "QuadTree.h"
#include <Windows.h>

std::vector<Spot> readSpotList(std::string);
double tod(std::string);

bool IntersectCircleInRectangule(Rect,Circle);
bool CirclPointIntersect(Circle cir, Spot *point);
Point GetCentreOfRect(Rect);
double dist(Spot , Spot * );

void PointsInCircleSearch(Node* , Spot , double , std::string , std::string , std::vector< Spot* >& );

void search(Node*, Circle, int*);

int main(int argc, char** argv)
{
    SetConsoleOutputCP(65001);
    if (argc < 5)
    {
        std::cout << "Not enought arguments!" << std::endl;
    }
    else {

        //Читаємо параметри
        std::string sourceFile = argv[1];
        double lon = stod(std::string(argv[3]));
        double lat = stod(std::string(argv[2]));
        double radius = stod(std::string(argv[4]));

        sourceFile = "spots.csv";

        // Переводимо параметри

        Point O = {
                6371.0 * cos(lat * 3.14159 / 180.) * (lon * 3.14159 / 180.),
                (lat * 3.14159 / 180.) * 6371.0
        };
        Circle searchArea = { radius, O };

        // Будуємо дерево
        QuadTree tree(22, 44, 41, 52);
        std::vector<Spot> geoSpots;

        geoSpots = readSpotList(sourceFile);

        for (int i = 0; i < geoSpots.size(); i++)
            tree.insert(&geoSpots[i]);


        //tree.print();

        // Ведемо пошук
        std::cout << "~ Here are the places we managed to find ~" << std::endl;
        search(tree.root, searchArea, 0);



    }
    return 0;
}

std::vector<Spot> readSpotList(std::string path){
    std::ifstream file(path);

    if (!file.is_open()){
        std::cout << "Failed to open spot list file!" << std::endl;
    }

    std::string buff;
    std::vector<Spot> geoSpots;

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

double tod(std::string str) {
    str[str.find(',')] = '.';

    return stod(str);
}

bool CirclPointIntersect(Circle cir, Spot *point) {
    bool res = pow(cir.centre.x - point->x, 2) + pow(cir.centre.y - point->y, 2) <= pow(cir.rad, 2);
    return res;
}

Point GetCentreOfRect(Rect rect)
{
    Point centre;
    centre.x = (rect.RT.x+rect.LB.x)/2;
    centre.y = (rect.RT.y+rect.LB.y)/2;
    return centre;
}

bool IntersectCircleInRectangule(Rect rec,Circle cir)
{
    Point centre = GetCentreOfRect(rec);
    if(centre.x == cir.centre.x && centre.y == cir.centre.y)
        return true;
    double  xn = std::max(rec.LB.x,std::min(cir.centre.x,rec.RT.x)),
            yn = std::max(rec.LB.y,std::min(cir.centre.y,rec.RT.y));
    double dx = xn - cir.centre.x,dy = yn - cir.centre.y;
    return (dx*dx + dy*dy)<(cir.rad*cir.rad);
}

void PointsInCircleSearch(Node* node, Point centre, double R, std::string type, std::string subtype, std::vector< Spot* >& result) {
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



void PointsInCircleSearch(Node*, Spot, double, std::string, std::string, std::vector<Spot*>&)
{
}

void search(Node* N, Circle area, int* counter) {
    if (counter == 0)
    {
        counter = new int;
        *counter = 1;
    }

    if (N->isLeaf)
    {
        for (int i = 0; i < N->data.size(); i++)
        {
            if (CirclPointIntersect(area, N->data[i]))
            {
                if (N->data[i]->name.size() == 0) N->data[i]->name = "NO_DATA";
                if (N->data[i]->type.size() == 0) N->data[i]->type = "NO_DATA";
                if (N->data[i]->subtype.size() == 0) N->data[i]->subtype = "NO_DATA";
                if (N->data[i]->address.size() == 0) N->data[i]->address = "NO_DATA";

                std::cout << *counter << ")  ";
                std::cout << N->data[i]->name << "; ";
                std::cout<< N->data[i]->type << "; ";
                std::cout<< N->data[i]->subtype << "; ";
                std::cout << N->data[i]->longitude << "; ";
                std::cout << N->data[i]->latitude << "; ";
                std::cout << N->data[i]->address << std::endl;

                (*counter)++;
            }
        }
    }
    else {
        for (int i = 0; i <  N->childs.size(); i++)
        {
            if (IntersectCircleInRectangule(N->childs[i]->MBR, area))
            {
                search(N->childs[i], area, counter);
            }
        }
    }
}