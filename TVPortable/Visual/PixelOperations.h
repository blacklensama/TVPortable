//
//  PixelOperations.h
//  TVPortable
//
//  Created by Ruiwei Bu on 6/17/12.
//  Copyright (c) 2012 Studio Symphonie. All rights reserved.
//

#ifndef TVPortable_PixelOperations_h
#define TVPortable_PixelOperations_h

#include "Prerequisites.h"

TVP_NS_VISUAL_BEGIN

void operate_pixels(uint32* src, 
                    int32 sleft, int32 stop, int32 swidth, int32 sheight,
                    uint32* dst, 
                    int32 dleft, int32 dtop, int32 dwidth, int32 dheight,
                    int operation);

TVP_NS_VISUAL_END

#endif
