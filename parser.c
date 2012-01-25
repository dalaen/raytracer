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
#define initCamera(which) which.name = NULL
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

void parse(struct Scene *outScene, struct Material **outMaterial, long* nbMaterials, struct Sphere **outSphere, long* nbSpheres, struct LightPoint **outPointLight, long* nbPointsLight, struct Camera **outCamera, long* nbCameras, struct TriangleMesh** outTriangle, long* nbTriangles)
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
        
        // All those giant lines could maybe be reduced in an inline function...
        if (strstr(trim, "material") != NULL)
        {
            if (*nbMaterials == 0)
                *outMaterial = (struct Material*) malloc(sizeof(struct Material));
            else if (*nbMaterials > 0)
                *outMaterial = (struct Material*) realloc(*outMaterial, sizeof(struct Material) * (*nbMaterials + 1));
            (*outMaterial)[(*nbMaterials)++] = parse_material(&nbBracketsOpen);
        }
        if (strstr(trim, "sphere") != NULL)
        {
            if (*nbSpheres == 0)
                *outSphere = (struct Sphere*) malloc(sizeof(struct Sphere));
            else if (*nbSpheres > 0)
                *outSphere = (struct Sphere*) realloc(*outSphere, sizeof(struct Sphere) * (*nbSpheres + 1));
            (*outSphere)[(*nbSpheres)++] = parse_sphere(&nbBracketsOpen);
        }
        if (strstr(trim, "point_light") != NULL)
        {
            if (*nbPointsLight == 0)
                *outPointLight = (struct LightPoint*) malloc(sizeof(struct LightPoint));
            else if (*nbPointsLight > 0)
                *outPointLight = (struct LightPoint*) realloc(*outPointLight, sizeof(struct LightPoint) * (*nbPointsLight + 1));
            (*outPointLight)[(*nbPointsLight)++] = parse_pointLight(&nbBracketsOpen);
        }
        if (strstr(trim, "camera") != NULL)
        {
            if (*nbCameras == 0)
                *outCamera = (struct Camera*) malloc(sizeof(struct Camera));
            else if (*nbCameras > 0)
                *outCamera = (struct Camera*) realloc(*outCamera, sizeof(struct Camera) * (*nbCameras + 1));
            (*outCamera)[(*nbCameras)++] = parse_camera(&nbBracketsOpen);
        }
        if (strstr(trim, "triangle_mesh") != NULL)
        {
            if (*nbTriangles == 0)
                *outTriangle = (struct TriangleMesh*) malloc(sizeof(struct TriangleMesh));
            else if (*nbTriangles > 0)
                *outTriangle = (struct TriangleMesh*) realloc(*outTriangle, sizeof(struct TriangleMesh) * (*nbTriangles + 1));
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
            result.name = clean_strdup(currentLine.attributeValue.stringAttribute);
        else if (strcmp("diffuse_color", currentLine.attributeName) == 0)
        {
            result.diffuseColor.red = currentLine.attributeValue.floatArrayAttribute[0];
            result.diffuseColor.green = currentLine.attributeValue.floatArrayAttribute[1];
            result.diffuseColor.blue = currentLine.attributeValue.floatArrayAttribute[2];
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
            result.name = clean_strdup(currentLine.attributeValue.stringAttribute);
        else if (strcmp("position", currentLine.attributeName) == 0)
            result.origin = array2position(currentLine.attributeValue.floatArrayAttribute);
        else if (strcmp("radius", currentLine.attributeName) == 0)
            result.radius = currentLine.attributeValue.floatAttribute;
        else if (strcmp("material", currentLine.attributeName) == 0)
            result.materialName = clean_strdup(currentLine.attributeValue.stringAttribute);
        
        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}

struct LightPoint parse_pointLight(unsigned long* nbBracketsOpen)
{
    struct LightPoint result;
    struct LineData currentLine;
    char buffer[MAX_LINE_SIZE];
    char* trim;
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
            result.name = clean_strdup(currentLine.attributeValue.stringAttribute);
        else if (strcmp("position", currentLine.attributeName) == 0)
            result.position = array2position(currentLine.attributeValue.floatArrayAttribute);
        else if (strcmp("color_intensity", currentLine.attributeName) == 0)
        {
            result.colorIntensity.red = currentLine.attributeValue.floatArrayAttribute[0];
            result.colorIntensity.green = currentLine.attributeValue.floatArrayAttribute[1];
            result.colorIntensity.blue = currentLine.attributeValue.floatArrayAttribute[2];
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
            result.name = clean_strdup(currentLine.attributeValue.stringAttribute);
        else if (strcmp("position", currentLine.attributeName) == 0)
            result.position = array2position(currentLine.attributeValue.floatArrayAttribute);
        else if (strcmp("look_at", currentLine.attributeName) == 0)
            result.lookAt = array2position(currentLine.attributeValue.floatArrayAttribute);
        else if (strcmp("projection_type", currentLine.attributeName) == 0)
        {
            if (strcmp(currentLine.attributeValue.stringAttribute, "orthographic") == 0)
                result.projectionType = ORTHOGRAPHIC;
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
    //static long nbFaces = 0;
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
            result.name = clean_strdup(currentLine.attributeValue.stringAttribute);
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
            result.position = array2position(currentLine.attributeValue.floatArrayAttribute);            

        free(currentLine.attributeName);
    } while (*nbBracketsOpen > onCall_bracketsOpen);
    
    return result;
}


void parse_render(char** sceneFil, struct Camera** outCamera, const long* nbCameras, struct OutputInfo* output)
{
    char buffer[MAX_LINE_SIZE];
    char* trim;
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
            *sceneFil = clean_strdup(currentLine.attributeValue.stringAttribute);
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
        fprintf(stderr, "\nNo camera was specified in the render configuration file... Cannot process to any rendering");
}




struct LineData parse_line(char* buffer)
{
    struct LineData result;
    char* argument;
    char* stringArg;
    long isString = 0;
    long wasFloat = 0;
    long temp = 0;
    float tempf = 0;
    
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
        {
            result.attributeValue.floatAttribute = atof(argument);
            wasFloat = 1;
        }
        else
        {
            result.attributeValue.longAttribute = atol(argument);
            wasFloat = 0;
        }
    }
    if (nextArgument() != NULL && !isString) // Is it an array?
    {
        if (!wasFloat)
        {
            temp = result.attributeValue.longAttribute;
            result.attributeValue.arrayAttribute[0] = temp;
            result.attributeValue.arrayAttribute[1] = atol(argument);
            if (nextArgument() != NULL)
                result.attributeValue.arrayAttribute[2] = atol(argument);
        }
        else if (wasFloat)
        {
            tempf = result.attributeValue.floatAttribute;
            result.attributeValue.floatArrayAttribute[0] = tempf;
            result.attributeValue.floatArrayAttribute[1] = atof(argument);
            if (nextArgument() != NULL)
                result.attributeValue.floatArrayAttribute[2] = atof(argument);
        }
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

unsigned long count_indent(char* theString)
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

void freeStructs(struct Material *outMaterial, long nbMaterials, struct Sphere *outSphere, long nbSpheres, struct LightPoint *outPointLight, long nbPointsLight, struct Camera *outCamera, long nbCameras, struct TriangleMesh *outTriangle, long nbTriangles)
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

char* clean_strdup(char* source) // Inline or not inline ?
{
    char* temp;
    temp = strdup(source);
    free(source); // Good clean dog.
    return temp;
}

struct Point3D array2position(float input[])
{
    struct Point3D result;
    result.x = input[0];
    result.y = input[1];
    result.z = input[2];
    
    return result;
}