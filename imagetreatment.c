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

/*void buildImage(struct Image *imageppm, struct Scene myScene, struct LightRay rays[][IMAGE_WIDTH])
{
    long x;
    long y;
    long spheresToDo;
    
    for (spheresToDo = myScene.nbSphere ; spheresToDo >= 0 ; spheresToDo--)
    {
        for (y = 0 ; y < myScene.height ; y++)
            for (x = 0 ; x < myScene.width ; x++)
            {
                if (intersecting(rays[y][x], myScene))
                    ;
            }
    }
}*/

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