/* 
 * File:   rays.h
 * Author: xavier
 *
 * Created on 19. oktober 2011, 08:38
 */

#ifndef RAYS_H
#define	RAYS_H

#ifdef	__cplusplus
extern "C" {
#endif

struct LightRay set_ray(long, long, long, long, long, long);
struct Pixel init_color(unsigned char, unsigned char, unsigned char);
float intersection_sphere(struct Sphere, struct LightRay);
long findMaterialIdByName(struct Material*, char*, const long);
float dotP3D(struct Point3D, struct Point3D);
struct Point3D doOnAllPosition(struct Point3D, double, struct Point3D, double);
float min(float, float);


#ifdef	__cplusplus
}
#endif

#endif	/* RAYS_H */

