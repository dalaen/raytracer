#include <stdio.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "spheres.h"
#include "rays.h"

struct LightRay set_ray(float originX, float originY, float originZ, float directionX, float directionY, float directionZ)
{
    struct LightRay newRay;
    
    newRay.origin.x = originX;
    newRay.origin.y = originY;
    newRay.origin.z = originZ;
    newRay.direction.x = directionX;
    newRay.direction.y = directionY;
    newRay.direction.z = directionZ;
    
    return newRay;
}

struct Pixel init_color(unsigned char red, unsigned char blue, unsigned char green)
{
    struct Pixel result;
    
    result.red = red;
    result.blue = blue;
    result.green = green;
    
    return result;
}

long findMaterialIdByName(const struct Material* haystack, char* name, const long nbMaterials)
{
    long i;
    
    for (i = 0 ; i < nbMaterials ; i++)
        if (strcmp(haystack[i].name, name) == 0)
            return i;
    return -1;
}

inline float dotP3D(struct Point3D a, struct Point3D b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

inline float min(float a, float b)
{
    return (a > b) ? b : a;
}