/* 
 * File:   parser.h
 * Author: xavier
 *
 * Created on 8. november 2011, 19:20
 */

#ifndef PARSER_H
#define	PARSER_H

#ifdef	__cplusplus
extern "C"
{
#endif

struct Scene init_scene(long nbSphere, long width, long height);
void parse(struct Scene *, struct Material**, long*, struct Sphere**, long*, struct LightPoint**, long*, struct Camera**, long*, struct TriangleMesh**, long*);
struct Scene parse_scene(unsigned long*);
struct Material parse_material(unsigned long*);
struct Sphere parse_sphere(unsigned long*);
struct LightPoint parse_pointLight(unsigned long*);
struct Camera parse_camera(unsigned long*);
struct TriangleMesh parse_triangle(unsigned long*);
struct Vertex parse_vertex(unsigned long*);
void parse_render(char**, struct Camera**, const long*, struct OutputInfo*);
struct LineData parse_line(char*);
enum Operation getOperation(char*);
unsigned long count_indent(char*);
long is_comment(char*);
char* rtrim(char*);
char* ltrim(char*);

void freeStructs(struct Material*, long, struct Sphere*, long, struct LightPoint*, long, struct Camera*, long, struct TriangleMesh*, long);

char* clean_strdup(char*);
struct Point3D array2position(float[]);


#ifdef	__cplusplus
}
#endif

#endif	/* PARSER_H */

