//
//  Base64.h
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/10/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#ifndef Project_Unknown_Base64_h
#define Project_Unknown_Base64_h

#include "UKN/Platform.h"
#include "UKN/Util.h"

namespace ukn {
    
    Array<uint8> base64_decode(const ukn_string& input);
    ukn_string base64_encode(const uint8* bytes_to_encode, uint32 in_len);
    
} // namespace ukn


#endif
