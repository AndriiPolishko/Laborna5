#include <fstream>
#include "QuadTree.h"

vector<Spot> readSpotList(string);
double tod(string);

int main(int argc, char** argv)
{
    QuadTree tree(22, 44, 41, 52);
    vector<Spot> geoSpots;

    Spot cent;
    geoSpots = readSpotList("spots.csv");

    cout << geoSpots.size() << endl;
    for(int i = 0; i < geoSpots.size(); i++)
    {
        tree.insert(&geoSpots[i]);
    }

    tree.print();

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