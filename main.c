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

int main(int argc, char** argv)
{
    struct Scene myScene;
    struct Material* materials = NULL;
    long nbMaterials = 0;
    struct Sphere* spheres = NULL;
    long nbSpheres = 0;
    struct LightPoint* lightPoints = NULL;
    long nbLightPoints = 0;
    struct Camera* cameras = NULL;
    long nbCameras = 0;
    struct TriangleMesh* triangles = NULL;
    long nbTriangles = 0;

    struct OutputInfo output;
    char* sceneFile = NULL;
    
    char* sceneFilename = NULL;
    char* renderFilename = NULL;
    char** whichFilename[2] = {NULL};
    
    long i = 0;
    /*
    myImage.width = IMAGE_WIDTH;
    myImage.height = IMAGE_HEIGHT;
    myScene = init_scene(NB_SPHERE, IMAGE_WIDTH, IMAGE_HEIGHT);
    fillColor(&myImage, blackColor); // Remplir l'image de noir
    //*/
    
    // Should be moved to another file?
    /*************************************************************/
    /********************* CLI MANAGEMENT ************************/
    /*************************************************************/
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
    /*************************************************************/
    /****************** END CLI MANAGEMENT ***********************/
    /*************************************************************/
    
    if (sceneFilename != NULL)
    {
        if (freopen(sceneFilename, "r", stdin) == NULL)
        {
            printf("Erreur lors de l'import du fichier : %s", sceneFilename);
            return(EXIT_FAILURE);
        }
        parse(&myScene, &materials, &nbMaterials, &spheres, &nbSpheres, &lightPoints, &nbLightPoints, &cameras, &nbCameras, &triangles, &nbTriangles);
    }
    if (renderFilename != NULL)
    {
        if (freopen(renderFilename, "r", stdin) == NULL)
        {
            printf("Erreur lors de l'import du fichier : %s", renderFilename);
            return(EXIT_FAILURE);
        }
        parse_render(&sceneFile, &cameras, &nbCameras, &output);
    }
    
    makeOutput(output); // Next step : directly in the file ?
    // IMMA CHARGIN MAH LAZER
    buildImage(&output, materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints);
    
    free(renderFilename);
    free(sceneFilename);
    free(sceneFile);
    freeStructs(materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints, cameras, nbCameras, triangles, nbTriangles);
    
    return (EXIT_SUCCESS);
}

