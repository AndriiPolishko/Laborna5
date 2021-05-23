
#include "structures.h"
void search(double,double ,double ,string,string );
Point GetCentreOfRect(Rectangle);
bool IntersectCircleInRectangule(Rectangle,Circle);
int main(int argc, char** argv)
{
    string type,subtype;
    double radius,lat,lon;

    return 0;
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

void search(double N,double lat,double lon,string type,string subtype ="" )
{
    vector<Spot> res;

}

