//
//  Config.h
//  Project Unknown
//
//  Created by Robert Bu on 11/24/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_ConfigParser_h
#define Project_Unknown_ConfigParser_h

#include "UKN/Platform.h"
#include "UKN/Uncopyable.h"
#include "UKN/Resource.h"
#include "UKN/PreDeclare.h"

#include <vector>

namespace ukn {
    
    /**
     * Abstract config loader/writer
     * Currently only supports xml
     **/
    enum ConfigParserType {
        CPT_XML,
        CPT_JSON,
    };
    
    class UKN_API ConfigParser: public IResource, Uncopyable {       
    public:
        virtual ~ConfigParser() { }
        
        virtual bool open(ResourcePtr resource) = 0;

        virtual bool create() = 0;
        
        virtual const String& getName() const = 0;
        
        virtual StreamPtr  writeToStream(const char* indent="\n") const = 0;
        virtual ukn_string writeToString(const char* indent="\n") const = 0;
        
        virtual void close() = 0;
        
        virtual bool hasNode(const ukn_string& node) const = 0;
        virtual ukn_string getCurrentNodeName() const = 0;
        virtual ukn_string getCurrentNodePath() const = 0;
        
        /**
         * set current node to a node, / for root, otherwise use relative path
         * /config/window/ etc
         **/
        virtual bool toNode(const ukn_string& node) = 0;
        
        /**
         * set current node to first node child
         **/
        virtual bool toFirstChild(const ukn_string& name=ukn_string()) = 0;
        /**
         * set current node to next slibling child
         **/
        virtual bool toNextChild(const ukn_string& name=ukn_string()) = 0;
        /**
         * set current node to node parent
         **/
        virtual bool toParent() = 0;
        
        
        virtual bool hasAttribute(const ukn_string& attr) const = 0;
        /**
         * get all attributes of current node
         **/
        virtual std::vector<ukn_string> getAttributes() const = 0;
        
        /**
         * get node attribute as optinal string
         */
        virtual ukn_string getString(const ukn_string& attr, const ukn_string& opt = ukn_string()) const = 0;
        /**
         * get node attribute as optinal bool
         */
        virtual bool getBool(const ukn_string& attr, bool opt = false) const = 0;
        /**
         * get node attribute as optinal bool
         */
        virtual int32 getInt(const ukn_string& attr, int32 opt = 0) const = 0;
        /**
         * get node attribute as optinal bool
         */
        virtual float getFloat(const ukn_string& attr, float opt = 0.f) const = 0;
        
        
        /**
         * Begin a new node under current node
         **/
        virtual bool beginNode(const ukn_string& nodeName) = 0;
        
        virtual void endNode() = 0;
        
        /**
         * set the value of current node
         **/
        virtual void setValue(const ukn_string& val) = 0;
        /**
         * set a string attribute under current node
         **/
        virtual void setString(const ukn_string& attr, const ukn_string& val) = 0;
        /**
         * set a bool attribute under current node
         **/
        virtual void setBool(const ukn_string& attr, bool val) = 0;
        /**
         * set a int attribute under current node
         **/
        virtual void setInt(const ukn_string& attr, int32 val) = 0;
        /**
         * set a float attribute under current node
         **/
        virtual void setFloat(const ukn_string& attr, float val) = 0;
        
        UKN_API static ConfigParserPtr MakeEmptyParser(ConfigParserType type);
        UKN_API static ConfigParserPtr MakeParser(ResourcePtr resource);
    };
    
    
    
} // namespace ukn


#endif
