//
//  Config.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/ConfigParser.h"
#include "UKN/Resource.h"
#include "UKN/Stream.h"

#include "ConfigParserXml.h"

namespace ukn {
    
    UKN_API ConfigParserPtr ConfigParser::MakeParser(ResourcePtr resource) {
        if(*resource) {
            ConfigParserPtr ptr = MakeSharedPtr<ConfigParserXmlImpl>();
            if(ptr && ptr->open(resource)) {
                return ptr;
            }
        } else {
            return ConfigParserPtr();
        }
        
        return ConfigParserPtr();
    }

    UKN_API ConfigParserPtr ConfigParser::MakeEmptyParser(ConfigParserType type) {
        switch(type) {
            case CPT_XML: {
                ConfigParserPtr ptr = MakeSharedPtr<ConfigParserXmlImpl>();
                ptr->create();
                return ptr;
            }
            case CPT_JSON:
            default:
                break;
        }
        return ConfigParserPtr();
    }

    
} // namespace ukn