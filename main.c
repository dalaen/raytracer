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
#include <time.h>
#include "structs.h"
#include "rays.h"
#include "spheres.h"
#include "imagetreatment.h"
#include "parser.h"
#define getArrayLocationByI(i) 2*i+2
#define isCLIOption(thisOne, i) strcmp(argv[getArrayLocationByI(i) - 1], thisOne) == 0
#define getCLIFileExtension(i) strrchr(argv[getArrayLocationByI(i)], '.') + 1

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
        if (isCLIOption("-h", i)) // Display help
        {
            fprintf(stderr, "Welcome to raytracing 0.1.x\n\n");
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "raytracing -s scenefilename -r renderfilename > picture.ppm\n");
            fprintf(stderr, "raytracing -s scenefilename -r renderfilename -o picture.ppm\n\n");
            fprintf(stderr, "DESCRIPTION\n");
            fprintf(stderr, "raytracing is a small student program capable of render 3D pictures featuring spheres, using raytracing technique.\n");
            fprintf(stderr, "Output picture format supported : PPM, binary PPM\n\n");
            fprintf(stderr, "\t-s file.conf : file.conf is going to be read by the program to take scene informations\n");
            fprintf(stderr, "\t-r file.conf : file.conf is going to be read by the program to take rendering informations\n");
            fprintf(stderr, "\t-o file.ppm : file.ppm is going to be written by the program and be made an picture of\n");
            fprintf(stderr, "\t-h : displays the current help text\n\n");
            fprintf(stderr, "Scene and Render configuration files extensions are required to be .conf\n");
            fprintf(stderr, "Output picture file extension is required to be .ppm\n\n");
            fprintf(stderr, "This program has been written by Xavier Poirot in France as for a project in school. Year 2011/2012\n");
            return(EXIT_SUCCESS);
        }
        if (argv[getArrayLocationByI(i)])
        {
            if (isCLIOption("-s", i)) // Scene file following
            {
                whichFilename[i] = &sceneFilename;
                isConf[i] = true;
            }
            else if (isCLIOption("-r", i))
            {
                whichFilename[i] = &renderFilename;
                isConf[i] = true;
            }
            else if (isCLIOption("-o", i))
                whichFilename[i] = &outputFilename;
        }
        else
        {
            fprintf(stderr, "You must use options associated with a filename!\n");
            return(EXIT_FAILURE);
        }
    }
    while (--i >= 0)
    {
        if (isConf[i] && strcmp(getCLIFileExtension(i), "conf") == 0) // If file's extension is "conf"
            *whichFilename[i] = strdup(argv[getArrayLocationByI(i)]);
        else if (!isConf[i] && strcmp(getCLIFileExtension(i), "ppm") == 0)
            *whichFilename[i] = strdup(argv[getArrayLocationByI(i)]);
        else
            fprintf(stderr, "%s file has an incorrect extension. Allowed extensions: .conf, .ppm\n", argv[getArrayLocationByI(i)]);
    }
    /*************************************************************/
    /****************** END CLI MANAGEMENT ***********************/
    /*************************************************************/
    
    if (sceneFilename != NULL)
    {
        if (freopen(sceneFilename, "r", stdin) == NULL)
        {
            fprintf(stderr, "Cannot open following file: %s\n", sceneFilename);
            return(EXIT_FAILURE);
        }
        parse(&myScene, &materials, &nbMaterials, &spheres, &nbSpheres, &lightPoints, &nbLightPoints, &cameras, &nbCameras, &triangles, &nbTriangles);
    }
    if (renderFilename != NULL)
    {
        if (freopen(renderFilename, "r", stdin) == NULL)
        {
            fprintf(stderr, "Cannot open following file: %s\n", renderFilename);
            return(EXIT_FAILURE);
        }
        parse_render(&sceneFile, &cameras, &nbCameras, &output);
    }
    if (outputFilename != NULL)
    {
        if (freopen(outputFilename, "w", stdout) == NULL)
        {
            fprintf(stderr, "Cannot open following file: %s\n", renderFilename);
            return(EXIT_FAILURE);
        }
    }
    
    makeOutput(output); // Next step : directly in the file ?
    // IMMA CHARGIN MAH LAZER
    buildImage(output, materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints, cameras, nbCameras);
    
    free(renderFilename);
    free(sceneFilename);
    free(outputFilename);
    free(sceneFile);
    freeStructs(materials, nbMaterials, spheres, nbSpheres, lightPoints, nbLightPoints, cameras, nbCameras, triangles, nbTriangles);
    
    fprintf(stderr, "Program executed in: %.3lf s\n", (double)clock() / (double)CLOCKS_PER_SEC);
    
    return (EXIT_SUCCESS);
}

