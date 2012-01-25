/* 
 * File:   imagetreatment.h
 * Author: xavier
 *
 * Created on 5. oktober 2011, 09:20
 */

#ifndef IMAGETREATMENT_H
#define	IMAGETREATMENT_H

#ifdef	__cplusplus
extern "C" {
#endif

    void buildImage(const struct OutputInfo, const struct Material*, const long, const struct Sphere*, const long, const struct LightPoint*, const long, const struct Camera*, const long);
    void makeOutput(const struct OutputInfo);
    void printLine(const struct Pixel, const struct OutputInfo);


#ifdef	__cplusplus
}
#endif

#endif	/* IMAGETREATMENT_H */

