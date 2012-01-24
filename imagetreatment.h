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

    void setPixel(struct OutputInfo*, long, long, unsigned char, unsigned char, unsigned char);
    void buildImage(struct OutputInfo*, struct Material*, const long, struct Sphere*, const long);
    void fillColor(struct OutputInfo*, struct Pixel);
    void makeOutput(struct OutputInfo);


#ifdef	__cplusplus
}
#endif

#endif	/* IMAGETREATMENT_H */

