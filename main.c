/* 
 * File:   main.c
 * Author: xavier
 *
 * Created on 19. oktober 2011, 08:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "rays.h"
#include "spheres.h"
#include "imagetreatment.h"
#include "parser.h"
#define getArrayLocationByI(i) 2*i+2

/*
 * 
 */
int main(int argc, char** argv)
{
    struct Image myImage;
    struct Scene myScene;
    struct Material* materials = NULL;
    long nbMaterials = 0;
    struct Sphere* spheres = NULL;
    long nbSpheres = 0;
    struct PointLight* pointsLight = NULL;
    long nbPointsLight = 0;
    struct Camera* cameras = NULL;
    long nbCameras = 0;
    struct TriangleMesh* triangles = NULL;
    long nbTriangles = 0;
    
    struct OutputInfo output;
    char* sceneFile = NULL;
    
    char* sceneFilename = NULL;
    char* renderFilename = NULL;
    char** whichFilename[2] = {NULL};
    struct LightRay ray;
    struct Pixel blackColor;
    struct Distance distance;
    long i = 0;
    long ok;                    // Determines if it's ok to continue
    long x;
    long y;
    long distanceIntersection;
    
    blackColor = init_color(0, 0, 0);
    /*
    myImage.width = IMAGE_WIDTH;
    myImage.height = IMAGE_HEIGHT;
    myScene = init_scene(NB_SPHERE, IMAGE_WIDTH, IMAGE_HEIGHT);
    fillColor(&myImage, blackColor); // Remplir l'image de noir
    //*/
    
    // CLI management
    if (argc >= 3) // First argument
    {
        if (strcmp(argv[1], "-s") == 0) // Scene file following
        {
            whichFilename[i++] = &sceneFilename;
        }
        else if (strcmp(argv[1], "-r") == 0)
        {
            whichFilename[i++] = &renderFilename;
        }
    }
    if (argc >= 5) // Two arguments
    {
        if (strcmp(argv[3], "-s") == 0 && whichFilename[0] != &sceneFilename) // Scene file following
        {
            whichFilename[i++] = &sceneFilename;
        }
        else if (strcmp(argv[3], "-r") == 0 && whichFilename[0] != &renderFilename)
        {
            whichFilename[i++] = &renderFilename;
        }
    }

    while (--i >= 0)
    {
        if (strcmp(strrchr(argv[getArrayLocationByI(i)], '.') + 1, "conf") == 0) // If file's extension is "conf"
        {
            *whichFilename[i] = malloc(sizeof(char) * strlen(argv[getArrayLocationByI(i)]) + 1);
            strcpy(*whichFilename[i], argv[getArrayLocationByI(i)]);
        }
    }
    
    if (sceneFilename != NULL)
    {
        if (freopen(sceneFilename, "r", stdin) == NULL)
        {
            printf("Erreur lors de l'import du fichier : %s", sceneFilename);
            return(EXIT_FAILURE);
        }
        parse(&myScene, &materials, &nbMaterials, &spheres, &nbSpheres, &pointsLight, &nbPointsLight, &cameras, &nbCameras, &triangles, &nbTriangles);
        printf("\nsphere.position.x = %ld\n", triangles[0].vertexes[1].position.y);
    }
    if (renderFilename != NULL)
    {
        if (freopen(renderFilename, "r", stdin) == NULL)
        {
            printf("Erreur lors de l'import du fichier : %s", renderFilename);
            return(EXIT_FAILURE);
        }
        parse_render(&sceneFile, &cameras, &nbCameras, &output);
        printf("\noutput_size.height = %ld\n", output.height);
    }
    
    /*
    // Création scène
    myScene.spheres[0] = init_sphere(233, 290, 200, 100, MAX_COLOR, 0, 0);
    myScene.spheres[1] = init_sphere(407, 290, 200, 100, 0, MAX_COLOR, 0);
    myScene.spheres[2] = init_sphere(320, 140, 200, 100, 0, 0, MAX_COLOR);
    
    for (y = 0 ; y < myImage.height ; y++)
    {
        for (x = 0 ; x < myImage.width ; x++)
        {
            // Initialisation du rayon pour chaque pixel
            ray = set_ray(x, y, 0, 0, 0, 1);
            // Quel objet est intersecté par ce rayon
            distance.distance = IMAGE_DEPTH;
            distance.whatSphere = 0;
            for (i = 0 ; i < NB_SPHERE ; i++)
            {
                if ((distanceIntersection = intersection_sphere(myScene.spheres[i], ray)) != 0.0) // Il y a intersection
                {
                    if (distance.distance > distanceIntersection)
                    {
                        distance.distance = distanceIntersection;
                        distance.whatSphere = i;
                    }
                }
                if (distance.distance != IMAGE_DEPTH)
                    setPixel(&myImage, x, y, myScene.spheres[distance.whatSphere].color.red, myScene.spheres[distance.whatSphere].color.green, myScene.spheres[distance.whatSphere].color.blue);
            }
        }
    }
    
    
    makeOutput(&myImage);
    //*/
    
    free(renderFilename);
    free(sceneFilename);
    free(sceneFile);
    freeStructs(materials, nbMaterials, spheres, nbSpheres, pointsLight, nbPointsLight, cameras, nbCameras, triangles, nbTriangles);
    
    return (EXIT_SUCCESS);
}

