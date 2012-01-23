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

    void setPixel(struct Image *imageppm, long, long, unsigned char, unsigned char, unsigned char);
//    void buildImage(struct Image *imageppm, struct Scene, struct Rays rays[][IMAGE_WIDTH]);
    void fillColor(struct Image *imageppm, struct Pixel);
    void makeOutput(struct Image *imageppm);


#ifdef	__cplusplus
}
#endif

#endif	/* IMAGETREATMENT_H */

