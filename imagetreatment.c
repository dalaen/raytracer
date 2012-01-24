/* 
 * File:   imagetreatment.c
 * Author: xavier
 *
 * Created on 5. oktober 2011, 09:17
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "imagetreatment.h"
#include "rays.h"

void setPixel(struct OutputInfo *imageppm, long x, long y, unsigned char red, unsigned char green, unsigned char blue)
{
    imageppm->image[y][x].blue = 255 * blue;
    imageppm->image[y][x].green = 255 * green;
    imageppm->image[y][x].red = 255 * red;
}

// IMMA CHARGIN MAH LAZER
void buildImage(struct OutputInfo* output, struct Material* materials, const long nbMaterials, struct Sphere* spheres, const long nbSpheres)
{
    struct Distance distance;
    struct LightRay ray;
    long distanceIntersection;
    long materialId;
    long i;
    long x;
    long y;
    
    for (y = 0 ; y < output->height ; y++)
    {
        for (x = 0 ; x < output->width ; x++)
        {
            // Initialisation du rayon pour chaque pixel
            ray = set_ray(x, y, 0, 0, 0, 1);
            // Quel objet est intersecté par ce rayon
            distance.distance = IMAGE_DEPTH;
            distance.whatSphere = 0;
            for (i = 0 ; i < nbSpheres ; i++)
            {
                if ((distanceIntersection = intersection_sphere(spheres[i], ray)) != 0.0) // Il y a intersection
                {
                    if (distance.distance > distanceIntersection)
                    {
                        distance.distance = distanceIntersection;
                        distance.whatSphere = i;
                    }
                }
                if (distance.distance != IMAGE_DEPTH)
                {
                    materialId = findMaterialIdByName(materials, spheres[distance.whatSphere].materialName, nbMaterials);
                    setPixel(output, x, y, materials[materialId].diffuseColor.red, materials[materialId].diffuseColor.green, materials[materialId].diffuseColor.blue);
                }
            }
        }
    }
}

void fillColor(struct OutputInfo *imageppm, struct Pixel color)
{
    int x;
    int y;
    
    for (y = 0 ; y < imageppm->height ; y++)
    {
        for (x = 0 ; x < imageppm->width ; x++)
        {
            setPixel(imageppm, x, y, color.red, color.green, color.blue);
        }
    }
}

void makeOutput(struct OutputInfo *imageppm)
{
    int x;
    int y;
    printf("P3");
    printf("\n");
    printf("%ld %ld", imageppm->width, imageppm->height);
    printf("\n");
    printf("%d", MAX_COLOR);
    printf("\n");
    // Affichage des pixels
    for (y = 0 ; y < imageppm->height ; y++)
    {
        for (x = 0 ; x < imageppm->width ; x++)
        {
            printf("%d %d %d\n", imageppm->image[y][x].red, imageppm->image[y][x].green, imageppm->image[y][x].blue);
        }
        printf("\n");
    }
    
}