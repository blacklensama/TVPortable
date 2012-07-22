//
//  LocalizedStringTableLoader.cpp
//  Project Unknown
//
//  Created by Robert Bu on 7/22/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "ukn/LocalizedStringTableLoader.h"
#include "ukn/LocalizedStringTable.h"

#include "ukn/ConfigParser.h"
#include "ukn/Resource.h"

namespace ukn {
    
    bool LocalizedStringTableLoader::Load(const char* file, LocalizedStringTable& stringTable) {
        ukn::ConfigParserPtr parser = ukn::ConfigParser::MakeParser(ukn::ResourceLoader::Instance().loadResource(file));
        if(parser &&
           parser->toNode("strings")) {
            
            parser->toFirstChild();
            
            bool stringIdLoaded = false;
            do {
                std::string lanName = parser->getCurrentNodeName();
                if(!lanName.empty()) {
                    stringTable.addLanguage(lanName);
                    
                    parser->toFirstChild();
                    
                    do {
                        std::string stringId = parser->getCurrentNodeName();
                        if(!stringId.empty()) {
                            
                            std::string value = parser->getString("value");
                            
                            stringTable.addString(lanName, stringId, value);
                            
                            if(!stringIdLoaded)
                            /* add string id */
                                stringTable.mIdVector.push_back(stringId);
                        }
                        
                    } while(parser->toNextChild());
                    
                    parser->toParent();
                    
                    stringIdLoaded = true;
                }
                
            } while(parser->toNextChild());
            
            return true;
        }

        return false;
    }
    
    bool LocalizedStringTableLoader::Save(const char* file, const LocalizedStringTable& stringTable) {
        ukn::ConfigParserPtr parser = ukn::ConfigParser::MakeEmptyParser(ukn::CPT_XML);
        
        if(parser) {
            parser->beginNode("strings");
            
            for(LocalizedStringTable::LanguageMap::const_iterator it = stringTable.begin(),
                end = stringTable.end();
                it != end;
                ++it) {
                parser->beginNode(it->first);
                
                const StringMap& stringMap = it->second;
                
                for(StringMap::StringMapCont::const_iterator it = stringMap.begin(),
                    end = stringMap.end();
                    it != end;
                    ++it) {
                    parser->beginNode(it->first);
                    parser->setString("value", it->second);
                    parser->endNode(); // String->first;
                }
                
                parser->endNode();
            }
            
            parser->endNode();
            
            std::string content = parser->writeToString();
            
            FILE* pfile = fopen(file, "w+");
            if(!pfile)
                return false;
            
            fwrite(content.c_str(), 1, content.size(), pfile);
            fclose(pfile);
            
            return true;
        }
        return false;
    }
    
    
}