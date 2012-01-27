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

#include <stdbool.h>
#define IMAGE_DEPTH 1000         // Image's depth (3D)
#define MAX_COLOR 255           // Max value of color gradiant
    
    struct Point3D {
        float x;
        float y;
        float z;
    };
    
    struct Pixel
    {
        float red;
        float blue;
        float green;
    };
    
    struct Sphere {
        char* name;
        struct Point3D origin;
        float radius;
        struct Pixel color;
        char* materialName;
    };
    
    struct Scene {
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
        struct Pixel diffuseColor; // R G B
        float reflectionCoefficient;
    };
    
    struct LightPoint {
        char* name;
        struct Point3D position; // x,y,z
        struct Pixel colorIntensity;
    };
    
    enum ProjectionType
    {
        ORTHOGRAPHIC
    };
    
    struct Camera {
        char* name;
        struct Point3D position; // x,y,z
        struct Point3D lookAt; // x,y,z
        enum ProjectionType projectionType;
        bool inUse;
    };

    struct LightRay {
        struct Point3D origin;
        struct Point3D direction; // à revoir
    };
    
    struct TriangleMesh {
        char* name;
        long nbVertexes;
        long nbFaces;
        struct Vertex* vertexes;
    };

    struct Vertex {
        long id;
        struct Point3D position;
    };
    
    
    struct Distance
    {
        long whatSphere;
        float distance;
    };
    
    enum Operation
    {
        EQUAL, NOT_EQUAL, GREATER_THAN, SMALLER_THAN, EQUAL_GREATER_THAN, EQUAL_SMALLER_THAN
    };
    
    union AttributeValue
    {
        long longAttribute;
        long arrayAttribute[5];
        float floatArrayAttribute[5];
        char* stringAttribute;
        float floatAttribute;
    };
    
    struct LineData
    {
        char* attributeName;
        enum Operation operation;
        union AttributeValue attributeValue;
    };
    
    enum FileFormat
    {
        PPM, PPMB
    };
    
    struct OutputInfo
    {
        enum FileFormat format;
        long width;
        long height;
    };

#ifdef	__cplusplus
}
#endif

#endif	/* STRUCTS_H */

