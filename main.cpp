#include <fstream>
#include "QuadTree.h"
//Laborna5.exe spots.csv 50.60659 30.5436 20 shop car
vector<Spot> readSpotList(string);
double tod(string);



void PointsInCircleSearch(Node* node, Spot centre, double R, string type, string subtype, vector< Spot* >& result) ;

int main(int argc, char** argv)
{
    QuadTree tree(22, 44, 41, 52);
    vector<Spot> geoSpots;

    geoSpots = readSpotList("spots.csv");
    cout << geoSpots.size() << endl;
    for(int i = 0; i < geoSpots.size(); i++)
    {
        tree.insert(&geoSpots[i]);
    }

    double lati = atof(argv[2]);
    double longi = atof(argv[3]);
    double rad = atof(argv[4]);
    string type = argv[5],subtype = argv[6];

    vector<Spot*> res;
    tree.search(tree.root,rad,longi,lati,type,res);
    for(auto i = 0;i<res.size();i++)
    {
        cout<<res[i]->latitude<<"|"<<res[i]->longitude<<"|"<<res[i]->type;
        cout<<endl;
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





