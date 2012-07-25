//
//  CCLocalizedLabelTTF.h
//  TVPortable
//
//  Created by Robert Bu on 7/25/12.
//
//

#ifndef __TVPortable__CCLocalizedLabelTTF__
#define __TVPortable__CCLocalizedLabelTTF__

#include "CCLabelTTF.h"
#include "CCLabelBMFont.h"
#include <string>

namespace cocos2d {
    
    class CCLocalizedLabel: public CCLabelTTF {
    public:
        static CCLabelTTF* LocalizedLabelTTFFromStringId(const std::string& sid, const char* fnt, size_t fontSize);
        static CCLabelTTF* LocalizedLabelTTFFromStringId(const std::string& sid, const CCSize& dimensions, CCTextAlignment alignment, const char* fnt, size_t fontSize);
        
        static CCLabelBMFont* LocalizedLabelBMFontFromStringId(const std::string& sid, const char* fnt);
    };
    
}


#endif /* defined(__TVPortable__CCLocalizedLabelTTF__) */
