//
//  ConfigParserXml.h
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_ConfigParserXml_h
#define Project_Unknown_ConfigParserXml_h

#include "UKN/ConfigParser.h"
#include "pugixml/pugixml.hpp"

namespace ukn {
    
    class ConfigParserXmlImpl: public ConfigParser {
    public:
        ConfigParserXmlImpl();
        virtual ~ConfigParserXmlImpl();
        
        virtual bool open(ResourcePtr resource);
        
        virtual bool create();
        
        virtual StreamPtr  writeToStream(const char* indent="\n") const;
        virtual ukn_string writeToString(const char* indent="\n") const;
        
        virtual void close();
        
        virtual bool        hasNode(const ukn_string& node) const;
        virtual ukn_string  getCurrentNodeName() const;
        virtual ukn_string  getCurrentNodePath() const;
        
        virtual bool toNode(const ukn_string& node);
        
        virtual bool toFirstChild(const ukn_string& name=ukn_string());
        virtual bool toNextChild(const ukn_string& name=ukn_string());
        virtual bool toParent();
        
        virtual bool hasAttribute(const ukn_string& attr) const;
        virtual std::vector<ukn_string> getAttributes() const;
        
        virtual ukn_string  getString(const ukn_string& attr, const ukn_string& opt) const;
        virtual bool        getBool(const ukn_string& attr, bool opt) const;
        virtual int32       getInt(const ukn_string& attr, int32 opt) const;
        virtual float       getFloat(const ukn_string& attr, float opt) const;
        
        ukn_string  getFormattedString() const;
        
        virtual bool beginNode(const ukn_string& name);
        virtual void endNode();
        
        virtual void setValue(const ukn_string& val);
        virtual void setString(const ukn_string& attr, const ukn_string& val);
        virtual void setBool(const ukn_string& attr, bool val);
        virtual void setInt(const ukn_string& attr, int32 val);
        virtual void setFloat(const ukn_string& attr, float val);
        
        const String& getName() const;
        
    private:
        struct myWritter: public pugi::xml_writer {
            void write(const void* data, size_t size);
            ukn_string str;
            
            myWritter(const char* indent);
            
            ukn_string indent;
        };
        
        struct myStreamWritter: public pugi::xml_writer {
            myStreamWritter(const char* indent);
            
            void write(const void* data, size_t size);            
            StreamPtr stream;
            ukn_string indent;
        };
        
        String mName;

        pugi::xml_document* mDocument;
        pugi::xml_node mCurrNode;
    };
    
} // namespace ukn

#endif
