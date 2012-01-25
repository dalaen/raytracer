#include <math.h>
#include "structs.h"

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
