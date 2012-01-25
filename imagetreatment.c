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
#include "spheres.h"

// IMMA CHARGIN MAH LAZER
void buildImage(const struct OutputInfo output, const struct Material* materials, const long nbMaterials, const struct Sphere* spheres, const long nbSpheres, const struct LightPoint* lightPoints, const long nbLightPoints, const struct Camera* cameras, const long nbCameras)
{
    struct Distance distance;
    struct LightRay ray;
    struct Point3D intersection;
    struct Point3D normale;
    struct LightRay lightray;
    struct Point3D directionLightPoint;
    struct Pixel color;
    const long POV_OFFSET = -2000;          // Constitute the camera depth position
    long materialId;
    long cameraId;
    long i;
    long j;
    long x;
    long y;
    long reflectionDepthLevel;
    float distanceIntersection;
    float lambert;
    float reflectionCoefficient;
    float norme;
    float reflect;
    int inShadow = 0;
    
    cameraId = findCameraIdUsed(cameras, nbCameras);
    
    for (y = 0 ; y < output.height ; y++)
    {
        for (x = 0 ; x < output.width ; x++)
        {
            // Initializations...
            ray = set_ray(x, y, POV_OFFSET, 0, 0, 1);
            color = init_color(0, 0, 0);
            reflectionCoefficient = 1.0;
            reflectionDepthLevel = 0;
//          O o
//          /¯________________________
//          | IMMA FIRIN' MAH LAZOR!!!
//          \_¯¯¯¯¯
            do
            {
                distance.distance = IMAGE_DEPTH - POV_OFFSET; // Initialize distance to the farest possible, and we're gonna catch the closest this way
                distance.whatSphere = -1;
                for (i = 0 ; i < nbSpheres ; i++)
                {
                    if ((distanceIntersection = intersection_sphere(spheres[i], ray)) != 0.0) // Il y a intersection
                    {
                        if (distanceIntersection < distance.distance)
                        {
                            distance.distance = distanceIntersection;
                            distance.whatSphere = i;
                        }
                    }
                }
                
                // No spheres cut? No reflections to do then
                if (distance.whatSphere == -1)
                    break;
                
                // New source of light
                // TODO: clean those 3 lines into 1
                intersection.x = ray.origin.x + distance.distance * ray.direction.x;
                intersection.y = ray.origin.y + distance.distance * ray.direction.y;
                intersection.z = ray.origin.z + distance.distance * ray.direction.z;

                // Compute n normale vector - also can be called "newStart.direction"
                normale.x = intersection.x - spheres[distance.whatSphere].origin.x;
                normale.y = intersection.y - spheres[distance.whatSphere].origin.y;
                normale.z = intersection.z - spheres[distance.whatSphere].origin.z;
                
                // Normalize
                norme = dotP3D(normale, normale);
                if (norme == 0.0)
                    break;

                normale.x /= sqrt(norme);
                normale.y /= sqrt(norme);
                normale.z /= sqrt(norme);
                
                // Find the material of the closest Sphere to the PoV
                materialId = findMaterialIdByName(materials, spheres[distance.whatSphere].materialName, nbMaterials);

                // Sum of all light points
                for (j = 0 ; j < nbLightPoints ; j++)
                {
                    directionLightPoint.x = lightPoints[j].position.x - intersection.x;
                    directionLightPoint.y = lightPoints[j].position.y - intersection.y;
                    directionLightPoint.z = lightPoints[j].position.z - intersection.z;
                    if (dotP3D(normale, directionLightPoint) > 0.0)
                    {
                        norme = sqrt(dotP3D(directionLightPoint, directionLightPoint));
                        if (norme > 0.0)
                        {
                            lightray.origin = intersection;
                            lightray.direction.x = (1/norme) * directionLightPoint.x;
                            lightray.direction.y = (1/norme) * directionLightPoint.y;
                            lightray.direction.z = (1/norme) * directionLightPoint.z;
                            inShadow = 0;
                            // Shadows
                            for (i = 0 ; i < nbSpheres ; i++)
                            {
                                if (intersection_sphere(spheres[i], lightray) != 0.0) // Il y a intersection
                                {
                                    inShadow = 1;
                                    break;
                                }
                            }
                            if (!inShadow)
                            {
                                lambert = reflectionCoefficient * dotP3D(lightray.direction, normale);
                                color.red += lambert * lightPoints[j].colorIntensity.red * materials[materialId].diffuseColor.red;
                                color.green += lambert * lightPoints[j].colorIntensity.green * materials[materialId].diffuseColor.green;
                                color.blue += lambert * lightPoints[j].colorIntensity.blue * materials[materialId].diffuseColor.blue;
                            }
                        }
                    }
                }
                
                reflectionCoefficient *= materials[materialId].reflectionCoefficient;

                reflect = 2.0f * dotP3D(ray.direction, normale);
                ray.origin = intersection;
                ray.direction.x -= reflect * normale.x;
                ray.direction.y -= reflect * normale.y;
                ray.direction.z -= reflect * normale.z;

                reflectionDepthLevel++;
            } while (reflectionCoefficient > 0.0 && reflectionDepthLevel < 10);

            printLine(color, output);
            //setPixel(output, x, y, min(1.0, red), min(1.0, green), min(1.0, blue));
        }
    }
}

void makeOutput(const struct OutputInfo output)
{
    if (output.format == PPM)
    {
        printf("P3");
        printf("\n");
        printf("%ld %ld", output.width, output.height);
        printf("\n");
        printf("%d", MAX_COLOR);
        printf("\n");
    }
}

void printLine(const struct Pixel color, const struct OutputInfo output)
{
    if (output.format == PPM)
        printf("%d %d %d\n", (unsigned char)min(255.0, 255.0 * color.red), (unsigned char)min(255.0, 255.0 * color.green), (unsigned char)min(255.0, 255.0 * color.blue));
}