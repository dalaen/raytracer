#include <stdio.h>
#include <string.h>
#include <math.h>
#include "structs.h"
#include "rays.h"

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

float intersection_sphere(struct Sphere sphere, struct LightRay ray)
{
    float x;
    float y;
    float z;
    float b;
    float d;
    float t;

    x = sphere.origin.x - ray.origin.x;
    y = sphere.origin.y - ray.origin.y;
    z = sphere.origin.z - ray.origin.z;
    b = x * ray.direction.x + y * ray.direction.y + z * ray.direction.z;
    d = b*b - x*x - y*y - z*z + sphere.radius * sphere.radius;
    if (d < 0.0)
        return 0.0;
    /* No intersection */
    d = sqrt(d);
    t = b - d;
    if (t <= 0.1) {
         /* Avoid choosing a point */
        t = b + d;
        /* behind or too close to */
        if (t <= 0.1) 
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

float dotP3D(struct Point3D a, struct Point3D b)
{
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

struct Point3D doOnAllPosition(struct Point3D sourceOffset, double offset, struct Point3D sourceCoefficient, double coefficient)
{
    struct Point3D result;
    
    result.x = offset * sourceOffset.x + coefficient * sourceCoefficient.x;
    result.y = offset * sourceOffset.y + coefficient * sourceCoefficient.y;
    result.z = offset * sourceOffset.z + coefficient * sourceCoefficient.z;
    
    return result;
}

float min(float a, float b)
{
    return (a > b) ? b : a;
}