#include "structs.h"
#include <stdio.h>
#include <math.h>

struct LightRay set_ray(long originX, long originY, long originZ, long directionX, long directionY, long directionZ)
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

double intersection_sphere(struct Sphere sphere, struct LightRay ray)
{
    double x;
    double y;
    double z;
    double b;
    double d;
    double t;

    x = sphere.origin.x - ray.origin.x;
    y = sphere.origin.y - ray.origin.y;
    z = sphere.origin.z - ray.origin.z;
    b = x * ray.direction.x + y * ray.direction.y + z * ray.direction.z;
    d = b*b - x*x - y*y - z*z + pow(sphere.radius, 2);
    if (d <= 0.0)
        return 0.0;
    /* No intersection */
    d = sqrt(d);
    t = b - d;
    if (t <= 0.001) {
         /* Avoid choosing a point */
        t = b + d;
        /* behind or too close to */
        if (t <= 0.001) 
            return 0.0;
        /* the origin
        */
    }
    return t;

}

long findMaterialIdByName(struct Material* haystack, char* name, const long nbMaterials)
{
    long i;
    
    for (i = 0 ; i < nbMaterials ; i++)
        if (strcmp(haystack[i].name, name) == 0)
            return i;
    return -1;
}