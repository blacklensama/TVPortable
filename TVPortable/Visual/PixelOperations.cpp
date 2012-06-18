//
//  PixelOperations.cpp
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#include "PixelOperations.h"

#include <cmath>

TVP_NS_VISUAL_BEGIN

#define COLOR_ARGB(a,r,g,b)      ((uint32(a)<<24) + (uint32(r)<<16) + (uint32(g)<<8) + uint32(b))
#define COLOR_GETA(col)          (((col)>>24) & 0xFF)
#define COLOR_GETR(col)          (((col)>>16) & 0xFF)
#define COLOR_GETG(col)          (((col)>>8) & 0xFF)
#define COLOR_GETB(col)          ((col) & 0xFF)
#define COLOR_SETA(col,a)	 	 (((col) & 0x00FFFFFF) + (uint32(a)<<24))
#define COLOR_SETR(col,r)	  	 (((col) & 0xFF00FFFF) + (uint32(r)<<16))
#define COLOR_SETG(col,g)		 (((col) & 0xFFFF00FF) + (uint32(g)<<8))
#define COLOR_SETB(col,b)		 (((col) & 0xFFFFFF00) + uint32(b))

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define BLEND(a, b, r) (a) * (1.0 - r) + b * r
#define OVERLAY(a, b) (a) < 0.5 ? (a * b * 2.0) : (1.0 - (1.0 - (a))) * (1.0 - (b)) * 2.0
#define HARDLIGHT(a, b) (b) < 0.5 ? (a * b * 2.0) : (1.0 - (1.0 - (a))) * (1.0 - (b)) * 2.0
#define SOFTLIGHT(a, b) (b) < 0.5 ? pow(a, 0.5 / (b)) : pow(a, (1.0 - (b)) / 2)
#define ABS(a) (a) < 0 ? (a) : -(a)

void operate_pixels(uint32* src, 
                    int32 sleft, int32 stop, int32 swidth, int32 sheight,
                    uint32* dst, 
                    int32 dleft, int32 dtop, int32 dwidth, int32 dheight,
                    float alpha,
                    int operation) {
    
}

float do_blend(float src, float dst, float alpha, int operation) {
    switch(operation) {
        case ltOpaque:          return src;
        case ltAlpha:           return BLEND(dst, src, alpha);
        case ltAddApha:         return MIN(1.0, dst * (1.0 - alpha) + src);
        case ltAdditive:        return MIN(1.0, dst + src);
        case ltSubtractive:     return MAX(0.0, dst + src - 1.0);
        case ltMultiplicative:  return dst * src;
        case ltDodge:           return MIN(1.0 , dst / (1.0 - src));
        case ltLighten:         return MAX(dst, src);
        case ltDarken:          return MIN(dst, src);
        case ltScreen:          return 1.0 - (1.0 - dst) * (1.0 - src);
        case ltPsNormal:        return dst * (1.0 - alpha) + src * alpha;
        case ltPsAdditive:      return BLEND(dst, MIN(1.0, dst + src), alpha);
        case ltPsSubtractive:   return BLEND(dst, MAX(0.0, dst + src - 1.0), alpha);
        case ltPsMultiplicative:return BLEND(dst, dst * src, alpha);
        case ltPsScreen:        return BLEND(dst, 1.0 - (1.0 - dst) * (1.0 - src), alpha);
        case ltPsOverlay:       return BLEND(dst, OVERLAY(dst, src), alpha);
        case lsPsHardLight:     return BLEND(dst, HARDLIGHT(dst, src), alpha);
        case ltPsSoftLight:     return BLEND(dst, SOFTLIGHT(dst, src), alpha);
        case lsPsColorDodge:    return BLEND(dst, MIN(1.0, dst / (1.0 - src)), alpha);
        case ltPsColorDodge5:   return MIN(1.0, dst / (1.0 - src * alpha));
        case ltPsColorBurn:     return BLEND(dst, MAX(0.0, 1.0 - (1.0 - dst) / src), alpha);
        case lsPsLighten:       return BLEND(dst, MAX(dst, src), alpha);
        case lsPsDarken:        return BLEND(dst, MIN(dst, src), alpha);
        case lsPsDifference:    return BLEND(dst, abs(dst - src), alpha);
        case lsPsDifference5:   return ABS(dst - src * alpha);
        case ltPsExclusion:     return BLEND(dst, dst + src - 2.0 * src * dst, alpha);
    }
}

TVP_NS_VISUAL_END
