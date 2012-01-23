#include "structs.h"

struct Sphere init_sphere(long originX, long originY, long originZ, float radius, long colorR, long colorG, long colorB)
{
    struct Sphere mySphere;
    mySphere.origin.x = originX;
    mySphere.origin.y = originY;
    mySphere.origin.z = originZ;
    mySphere.radius = radius;
    mySphere.color.red = colorR;
    mySphere.color.green = colorG;
    mySphere.color.blue = colorB;
    
    return mySphere;
}
