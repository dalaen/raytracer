/* 
 * File:   structs.h
 * Author: xavier
 *
 * Created on 19. oktober 2011, 08:27
 */

#ifndef STRUCTS_H
#define	STRUCTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NB_SPHERE 3             // Number of spheres in the scene
#define IMAGE_WIDTH 640         // Image's width
#define IMAGE_HEIGHT 480        // Image's height
#define IMAGE_DEPTH 500         // Image's depth (3D)
#define MAX_COLOR 255           // Max value of color gradiant
    
    struct Point3D {
        long x;
        long y;
        long z;
    };
    
    struct Pixel
    {
        unsigned char red;
        unsigned char blue;
        unsigned char green;
    };
    
    struct Sphere {
        char* name;
        struct Point3D origin;
        float radius;
        struct Pixel color;
        char* materialName;
    };
    
    struct Scene {
        struct Sphere spheres[NB_SPHERE];
        unsigned long nbSphere;
        unsigned long nbMaterial;
        unsigned long nbTriangleMesh;
        unsigned long nbPointLight;
        unsigned long nbCamera;
        unsigned long width;
        unsigned long height;
    };
    
    struct Material {
        char* name;
        unsigned int diffuseColor[3]; // R G B
        float reflectionCoefficient;
    };
    
    struct PointLight {
        char* name;
        struct Point3D position; // x,y,z
        long colorIntensity[3];
    };
    
    struct Camera {
        char* name;
        struct Point3D position; // x,y,z
        struct Point3D lookAt; // x,y,z
        char* projectionType;
    };

    struct LightRay {
        struct Point3D origin;
        struct Point3D direction; // à revoir
    };
    
    struct Image
    {
        long width;
        long height;
        struct Pixel image[IMAGE_HEIGHT][IMAGE_WIDTH];
    };
    
    struct Distance
    {
        long whatSphere;
        double distance;
    };
    
    enum Operation
    {
        EQUAL, NOT_EQUAL, GREATER_THAN, SMALLER_THAN, EQUAL_GREATER_THAN, EQUAL_SMALLER_THAN
    };
    
    union AttributeValue
    {
        unsigned long longAttribute;
        unsigned long arrayAttribute[5];
        char* stringAttribute;
        float floatAttribute;
    };
    
    struct LineData
    {
        char* attributeName;
        enum Operation operation;
        union AttributeValue attributeValue;
    };

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTS_H */

