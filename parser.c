#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "parser.h"
#define MAX_LINE_SIZE 1024
#define readLine() fgets(buffer, MAX_LINE_SIZE, stdin)
#define trim(what) ltrim(rtrim(what))
#define nextArgument() (argument = strtok(NULL, " "))
#define initMaterial(which) which.name = NULL
#define initSphere(which) which.name = NULL; which.materialName = NULL
#define initPointLight(which) which.name = NULL
#define initCamera(which) which.name = NULL; which.projectionType = NULL
#define initTriangleMesh(which) which.name = NULL; which.vertexes = NULL
#define initFace(which, by) which.type = by

struct Scene init_scene(long nbSphere, long width, long height)
{
    struct Scene myNewScene;
    myNewScene.nbSphere = nbSphere;
    myNewScene.width = width;
    myNewScene.height = height;
    
    return myNewScene;
}

void parse(struct Scene *outScene, struct Material **outMaterial, long* nbMaterials, struct Sphere **outSphere, long* nbSpheres, struct PointLight **outPointLight, long* nbPointsLight, struct Camera **outCamera, long* nbCameras, struct TriangleMesh** outTriangle, long* nbTriangles)
{
    char buffer[MAX_LINE_SIZE];
    char* trim;
    unsigned long nbBracketsOpen = 0;
    
    while (readLine() != NULL)
    {
        trim = buffer;
        trim = trim(trim);

        // First the scene
        if (strstr(trim, "scene") != NULL)
            *outScene = parse_scene(&nbBracketsOpen);
        if (strstr(trim, "material") != NULL)
        {
            if (*nbMaterials == 0)
                *outMaterial = (struct Material*) malloc(sizeof(struct Material));
            else if (*nbMaterials > 0)
                *outMaterial = (struct Material*) realloc(outMaterial, sizeof(struct Material) * (*nbMaterials + 1));
            (*outMaterial)[(*nbMaterials)++] = parse_material(&nbBracketsOpen);
        }
        if (strstr(trim, "sphere") != NULL)
        {
            if (*nbSpheres == 0)
                *outSphere = (struct Sphere*) malloc(sizeof(struct Sphere));
            else if (*nbSpheres > 0)
                *outSphere = (struct Sphere*) realloc(outSphere, sizeof(struct Sphere) * (*nbSpheres + 1));
            (*outSphere)[(*nbSpheres)++] = parse_sphere(&nbBracketsOpen);
        }
        if (strstr(trim, "point_light") != NULL)
        {
            if (*nbPointsLight == 0)
                *outPointLight = (struct PointLight*) malloc(sizeof(struct PointLight));
            else if (*nbPointsLight > 0)
                *outPointLight = (struct PointLight*) realloc(outPointLight, sizeof(struct PointLight) * (*nbPointsLight + 1));
            (*outPointLight)[(*nbPointsLight)++] = parse_pointLight(&nbBracketsOpen);
        }
        if (strstr(trim, "camera") != NULL)
        {
            if (*nbCameras == 0)
                *outCamera = (struct Camera*) malloc(sizeof(struct Camera));
            else if (*nbCameras > 0)
                *outCamera = (struct Camera*) realloc(outCamera, sizeof(struct Camera) * (*nbCameras + 1));
            (*outCamera)[(*nbCameras)++] = parse_camera(&nbBracketsOpen);
        }
        if (strstr(trim, "triangle_mesh") != NULL)
        {
            if (*nbTriangles == 0)
                *outTriangle = (struct TriangleMesh*) malloc(sizeof(struct TriangleMesh));
            else if (*nbTriangles > 0)
                *outTriangle = (struct TriangleMesh*) realloc(outTriangle, sizeof(struct TriangleMesh) * (*nbTriangles + 1));
            (*outTriangle)[(*nbTriangles)++] = parse_triangle(&nbBracketsOpen);
        }
    }
}

struct Scene parse_scene(unsigned long* nbBracketsOpen)
{
    struct Scene result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("number_of_materials", currentLine.attributeName) == 0)
            result.nbMaterial = currentLine.attributeValue.longAttribute;
        else if (strcmp("number_of_spheres", currentLine.attributeName) == 0)
            result.nbSphere = currentLine.attributeValue.longAttribute;
        else if (strcmp("number_of_triangle_meshes", currentLine.attributeName) == 0)
            result.nbTriangleMesh = currentLine.attributeValue.longAttribute;
        else if (strcmp("number_of_point_lights", currentLine.attributeName) == 0)
            result.nbPointLight = currentLine.attributeValue.longAttribute;
        else if (strcmp("number_of_cameras", currentLine.attributeName) == 0)
            result.nbCamera = currentLine.attributeValue.longAttribute;
        
        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);

    return result;
}

struct Material parse_material(unsigned long* nbBracketsOpen)
{
    struct Material result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    long i;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    initMaterial(result);
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("name", currentLine.attributeName) == 0)
        {
            result.name = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.name, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("diffuse_color", currentLine.attributeName) == 0)
        {
            for (i = 0 ; i < 3 ; i++)
                result.diffuseColor[i] = currentLine.attributeValue.arrayAttribute[i];
        }
        else if (strcmp("reflection_coefficient", currentLine.attributeName) == 0)
            result.reflectionCoefficient = currentLine.attributeValue.floatAttribute;

        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct Sphere parse_sphere(unsigned long* nbBracketsOpen)
{
    struct Sphere result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    initSphere(result);
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("name", currentLine.attributeName) == 0)
        {
            result.name = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.name, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("position", currentLine.attributeName) == 0)
        {
            result.origin.x = currentLine.attributeValue.arrayAttribute[0];
            result.origin.y = currentLine.attributeValue.arrayAttribute[1];
            result.origin.z = currentLine.attributeValue.arrayAttribute[2];
        }
        else if (strcmp("radius", currentLine.attributeName) == 0)
            result.radius = currentLine.attributeValue.floatAttribute;
        else if (strcmp("material", currentLine.attributeName) == 0)
        {
            result.materialName = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.materialName, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
        }
        
        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct PointLight parse_pointLight(unsigned long* nbBracketsOpen)
{
    struct PointLight result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    long i;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    initPointLight(result);
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("name", currentLine.attributeName) == 0)
        {
            result.name = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.name, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
            
        }
        else if (strcmp("position", currentLine.attributeName) == 0)
        {
            result.position.x = currentLine.attributeValue.arrayAttribute[0];
            result.position.y = currentLine.attributeValue.arrayAttribute[1];
            result.position.z = currentLine.attributeValue.arrayAttribute[2];
        }
        else if (strcmp("color_intensity", currentLine.attributeName) == 0)
        {
            for (i = 0 ; i < 2 ; i++)
                result.colorIntensity[i] = currentLine.attributeValue.arrayAttribute[i];
        }

        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct Camera parse_camera(unsigned long* nbBracketsOpen)
{
    struct Camera result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    long i;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    initCamera(result);
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("name", currentLine.attributeName) == 0)
        {
            result.name = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.name, currentLine.attributeValue.stringAttribute);   
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("position", currentLine.attributeName) == 0)
        {
            result.position.x = currentLine.attributeValue.arrayAttribute[0];
            result.position.y = currentLine.attributeValue.arrayAttribute[1];
            result.position.z = currentLine.attributeValue.arrayAttribute[2];
        }
        else if (strcmp("look_at", currentLine.attributeName) == 0)
        {
            result.lookAt.x = currentLine.attributeValue.arrayAttribute[0];
            result.lookAt.y = currentLine.attributeValue.arrayAttribute[1];
            result.lookAt.z = currentLine.attributeValue.arrayAttribute[2];
        }
        else if (strcmp("projection_type", currentLine.attributeName) == 0)
        {
            result.projectionType = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.projectionType, currentLine.attributeValue.stringAttribute);  
            free(currentLine.attributeValue.stringAttribute);
        }
        
        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct TriangleMesh parse_triangle(unsigned long* nbBracketsOpen)
{
    struct TriangleMesh result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    static long nbVertexDone = 0;
    static long nbFaces = 0;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    initTriangleMesh(result);
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("name", currentLine.attributeName) == 0)
        {
            result.name = malloc(sizeof(char) * (strlen(currentLine.attributeValue.stringAttribute) + 1));
            strcpy(result.name, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("number_of_vertexes", currentLine.attributeName) == 0)
            result.nbVertexes = currentLine.attributeValue.longAttribute;
        else if (strcmp("number_of_faces", currentLine.attributeName) == 0)
            result.nbFaces = currentLine.attributeValue.longAttribute;
        else if (strcmp("vertex", currentLine.attributeName) == 0)
        {
            if (nbVertexDone == 0)
                result.vertexes = (struct Vertex*) malloc(sizeof(struct Vertex));
            else if (nbVertexDone > 0)
                result.vertexes = (struct Vertex*) realloc(result.vertexes, sizeof(struct Vertex) * (nbVertexDone + 1));
            result.vertexes[nbVertexDone++] = parse_vertex(nbBracketsOpen);
        }

        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct Vertex parse_vertex(unsigned long* nbBracketsOpen)
{
    struct Vertex result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
    unsigned long onCall_bracketsOpen = *nbBracketsOpen;
    
    do
    {
        readLine();
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            (*nbBracketsOpen)++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            (*nbBracketsOpen)--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("id", currentLine.attributeName) == 0)
            result.id = currentLine.attributeValue.longAttribute;
        else if (strcmp("position", currentLine.attributeName) == 0)
        {
            result.position.x = currentLine.attributeValue.arrayAttribute[0];
            result.position.y = currentLine.attributeValue.arrayAttribute[1];
            result.position.z = currentLine.attributeValue.arrayAttribute[2];
        }
            

        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}


void parse_render(char* sceneFile, struct Camera** outCamera, const long* nbCameras, struct OutputInfo* output)
{
    char buffer[MAX_LINE_SIZE];
    char* trim;
    char* temp = NULL;
    struct LineData currentLine;
    long i;
    int cameraFound = 0;
    unsigned long nbBracketsOpen = 0;
    
    while (readLine() != NULL)
    {
        trim = buffer;
        trim = trim(trim);
        
        // Function is called when "scene" text is matched
        // Jumping over {
        if (*trim == '{')
        {
            nbBracketsOpen++;
            continue; // Next line
        }
        else if (*trim == '}')
        {
            nbBracketsOpen--;
            continue; // Next line
        }
        if (is_comment(trim)) // Jump over comments
            continue;
        if (strlen(trim) == 0) // Blank line? Jump!
            continue;
        
        currentLine = parse_line(trim);
        
        if (strcmp("scene_file", currentLine.attributeName) == 0)
        {
            sceneFile = malloc(sizeof(char) * strlen(currentLine.attributeValue.stringAttribute) + 1);
            strcpy(sceneFile, currentLine.attributeValue.stringAttribute);
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("camera", currentLine.attributeName) == 0)
        {
            for (i = 0 ; i < *nbCameras ; i++)
            {
                if (strcmp(currentLine.attributeValue.stringAttribute, outCamera[i]->name) == 0)
                {
                    outCamera[i]->inUse = 1;
                    cameraFound = 1;
                }
            }
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("output_format", currentLine.attributeName) == 0)
        {
            if (strcmp("PPM", currentLine.attributeValue.stringAttribute) == 0)
                output->format = PPM;
            else if (strcmp("BMP", currentLine.attributeValue.stringAttribute))
                output->format = BMP;
            else // default output format
                output->format = PPM;
            free(currentLine.attributeValue.stringAttribute);
        }
        else if (strcmp("output_size", currentLine.attributeName) == 0)
        {
            output->width = currentLine.attributeValue.arrayAttribute[0];
            output->height = currentLine.attributeValue.arrayAttribute[1];
        }
            
        free(currentLine.attributeName);
    }
    if (!cameraFound)
        printf("\nNo camera was specified in the render configuration file... Cannot process to any rendering");
}




struct LineData parse_line(char* buffer)
{
    struct LineData result;
    char* argument;
    char* stringArg;
    long isString = 0;
    long temp = 0;
    
    result.attributeValue.stringAttribute = NULL; // Initialization required
    if ((stringArg = strchr(buffer, '"')) != NULL) // there is a string
    {
        result.attributeValue.stringAttribute = malloc(sizeof(char) * (strlen(stringArg)));
        strcpy(result.attributeValue.stringAttribute, strchr(buffer, '"') + 1);
        result.attributeValue.stringAttribute[strlen(result.attributeValue.stringAttribute) - 1] = '\0';
        isString = 1;
    }
     
    argument = strtok(buffer, " ");
    result.attributeName = malloc(sizeof(char) * (strlen(argument) + 1));
    strcpy(result.attributeName, argument);
    
    if (nextArgument() != NULL) // More than 1 argument
        result.operation = getOperation(argument);
    
    if (nextArgument() != NULL)
    {
        if (isString) // Is it a string?
        {
            // Job already done, just keep the "else" working
        }
        else if (strchr(argument, '.') != NULL) // Is it a float number?
            result.attributeValue.floatAttribute = atof(argument);
        else
        {
            result.attributeValue.longAttribute = atol(argument);
        }
    }
    if (nextArgument() != NULL && !isString) // Is it an array?
    {
        temp = result.attributeValue.longAttribute;
        result.attributeValue.arrayAttribute[0] = temp;
        result.attributeValue.arrayAttribute[1] = atol(argument);
        if (nextArgument() != NULL)
            result.attributeValue.arrayAttribute[2] = atol(argument);
    }
    
    return result;
}

enum Operation getOperation(char* operation)
{
    if (strlen(operation) == 1) // One character
    {
        if (operation[0] == '=')
            return EQUAL;
        else if (operation[0] == '<')
            return SMALLER_THAN;
        else if (operation[0] == '>')
            return GREATER_THAN;
    }
    else if (strlen(operation) == 2) // Two characters
    {
        if (strcmp("!=", operation) == 0)
            return NOT_EQUAL;
        else if (strcmp(">=", operation) == 0)
            return EQUAL_GREATER_THAN;
        else if (strcmp("<=", operation) == 0)
            return EQUAL_SMALLER_THAN;
    }
    return NOT_EQUAL;
}

unsigned long count_indent(const char* theString)
{
    // This function is trimming too
    unsigned long count = 0;
    char* buffer = theString;
    
    while (*buffer++ == '\t')
        count++;
    
    return count;
}

long is_comment(char* theString)
{
    if ('#' == *theString) // Matching '#' ?
        return 1;
    return 0;
}

char* ltrim(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}

char* rtrim(char *s)
{
    char* back = s + strlen(s);
    while (isspace(*--back));
    back[1] = '\0';
    return s;
}

void freeStructs(struct Material *outMaterial, long nbMaterials, struct Sphere *outSphere, long nbSpheres, struct PointLight *outPointLight, long nbPointsLight, struct Camera *outCamera, long nbCameras, struct TriangleMesh *outTriangle, long nbTriangles)
{
    long i;
    
    
    if (outMaterial != NULL)
    {
        for (i = 0 ; i < nbMaterials ; i++)
        {
            free(outMaterial[i].name);
        }
        free(outMaterial);
    }
    if (outSphere != NULL)
    {
        for (i = 0 ; i < nbSpheres ; i++)
        {
            free(outSphere[i].name);
            free(outSphere[i].materialName);
        }
        free(outSphere);
    }
    if (outPointLight != NULL)
    {
        for (i = 0 ; i < nbPointsLight ; i++)
        {
            free(outPointLight[i].name);
        }
        free(outPointLight);
    }
    if (outCamera != NULL)
    {
        for (i = 0 ; i < nbCameras ; i++)
        {
            free(outCamera[i].name);
            free(outCamera[i].projectionType);
        }
        free(outCamera);
    }
    if (outTriangle != NULL)
    {
        for (i = 0 ; i < nbTriangles ; i++)
        {
            free(outTriangle[i].name);
            if (outTriangle[i].vertexes != NULL)
                free(outTriangle[i].vertexes);
        }
        free(outTriangle);
    }
}