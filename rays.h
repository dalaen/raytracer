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

struct LightRay set_ray(float, float, float, float, float, float);
struct Pixel init_color(unsigned char, unsigned char, unsigned char);
long findMaterialIdByName(const struct Material*, char*, const long);
inline float dotP3D(struct Point3D, struct Point3D);
inline float min(float, float);


#ifdef	__cplusplus
}
#endif

#endif	/* RAYS_H */

