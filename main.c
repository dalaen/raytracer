/* 
 * File:   main.c
 * Author: xavier
 *
 * Created on 19. oktober 2011, 08:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
    char* outputFilename = NULL;
    char** whichFilename[3] = {NULL};
    
    bool isConf[3] = {false};
    long i = 0;
    
    // Should be moved to another file?
    /*************************************************************/
    /********************* CLI MANAGEMENT ************************/
    /*************************************************************/
    for (i = 0 ; i < 0.5 * argc - 0.5; i++)
    {
        if (strcmp(argv[getArrayLocationByI(i) - 1], "-s") == 0) // Scene file following
        {
            whichFilename[i] = &sceneFilename;
            isConf[i] = true;
        }
        else if (strcmp(argv[getArrayLocationByI(i) - 1], "-r") == 0)
        {
            whichFilename[i] = &renderFilename;
            isConf[i] = true;
        }
        else if (strcmp(argv[getArrayLocationByI(i) - 1], "-o") == 0)
            whichFilename[i] = &outputFilename;
    }
    while (--i >= 0)
    {
        if (isConf[i] && strcmp(strrchr(argv[getArrayLocationByI(i)], '.') + 1, "conf") == 0) // If file's extension is "conf"
            *whichFilename[i] = strdup(argv[getArrayLocationByI(i)]);
        else if (!isConf[i] && (strcmp(strrchr(argv[getArrayLocationByI(i)], '.') + 1, "ppm") == 0 || strcmp(strrchr(argv[getArrayLocationByI(i)], '.') + 1, "bmp") == 0))
            *whichFilename[i] = strdup(argv[getArrayLocationByI(i)]);
        else
            printf("%s file has an incorrect extension. Allowed extensions: .conf, .ppm, .bmp\n", argv[getArrayLocationByI(i)]);
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
    buildImage(output, materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints);
    
    free(renderFilename);
    free(sceneFilename);
    free(outputFilename);
    free(sceneFile);
    freeStructs(materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints, cameras, nbCameras, triangles, nbTriangles);
    
    return (EXIT_SUCCESS);
}

