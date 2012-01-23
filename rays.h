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
double intersection_sphere(struct Sphere, struct LightRay);


#ifdef	__cplusplus
}
#endif

#endif	/* RAYS_H */

