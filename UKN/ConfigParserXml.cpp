//
//  ConfigParserXml.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/30/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "ConfigParserXml.h"
#include "UKN/Stream.h"
#include "UKN/Resource.h"
#include "UKN/StringUtil.h"
#include "UKN/SysUtil.h"

namespace ukn {
    
    ConfigParserXmlImpl::ConfigParserXmlImpl():
    mDocument(0) {
        
    }
    
    ConfigParserXmlImpl::~ConfigParserXmlImpl() {
        close();
    }
    
    bool ConfigParserXmlImpl::open(ResourcePtr resource) {
        StreamPtr dataStream = resource->getResourceStream()->readIntoMemory();
        if(dataStream) {
            if(!mDocument) {
                mDocument = new pugi::xml_document;
            }
            bool result = mDocument->load_buffer((void*)static_cast<MemoryStream*>(dataStream.get())->data(), dataStream->size(), pugi::encoding_wchar);
            if(result) {
                mName = resource->getName();
                mCurrNode = mDocument->root();
                return result;
            }
        }
        return false;
    }

    bool ConfigParserXmlImpl::create() {
        if(mDocument) {
            delete mDocument;
        }
        mDocument = new pugi::xml_document;
        mCurrNode = mDocument->root();
        return true;
    }
    
    void ConfigParserXmlImpl::close() {
        if(mDocument) {
            delete mDocument;
            mDocument = 0;
        }
    }
    
    bool ConfigParserXmlImpl::hasNode(const ukn_string& node) const {
        if(!mDocument)
            return false;
        
        pugi::xml_node nextNode = node[0] == '/' ? mCurrNode.root() : mCurrNode;
        
        StringTokenlizer tokens(node, "/");
        if(tokens.size() == 0)
            return false;
        
        StringTokenlizer::iterator it = tokens.begin();
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
                continue;
            }
            
            nextNode = nextNode.child((*it).c_str());
            if(!nextNode)
                return false;
            ++it;
        }
        return true;
    }
    
    ukn_string ConfigParserXmlImpl::getCurrentNodeName() const {
        if(mDocument)
            return mCurrNode.name();
        return ukn_string();
    }
    
    ukn_string ConfigParserXmlImpl::getCurrentNodePath() const {
        if(mDocument)
            return mCurrNode.path();
        return ukn_string();
    }
    
    bool ConfigParserXmlImpl::toNode(const ukn_string& node) {
        if(!mDocument)
            return false;
        
        if(node.size() == 1 && node[0] == '/') {
            mCurrNode = mDocument->root();
            return true;
        }
        
        StringTokenlizer tokens(node, "/");
        if(tokens.size() == 0)
            return false;
        
        pugi::xml_node nextNode = node[0] == '/' ? mCurrNode.root() : mCurrNode;
        StringTokenlizer::iterator it = tokens.begin();
        while(it != tokens.end()) {
            if((*it).size() == 0) {
                ++it;
                continue;
            }
            
            nextNode = nextNode.child((*it).c_str());
            if(!nextNode)
                return false;
            ++it;
        }
        
        mCurrNode = nextNode;
        return true;
    }
    
    bool ConfigParserXmlImpl::toFirstChild(const ukn_string& name) {
        if(!mDocument)
            return false;
        
        pugi::xml_node node;
        if(name.empty()) {
            node = mCurrNode.first_child();
        } else {
            node = mCurrNode.child(name.c_str());
        }
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool ConfigParserXmlImpl::toNextChild(const ukn_string& name) {
        if(!mDocument)
            return false;
        
        pugi::xml_node node;
        if(name.empty()) {
            node = mCurrNode.next_sibling();
        } else {
            node = mCurrNode.next_sibling(name.c_str());
        }
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool ConfigParserXmlImpl::toParent() {
        if(!mDocument)
            return false;
        
        pugi::xml_node node = mCurrNode.parent();
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    bool ConfigParserXmlImpl::hasAttribute(const ukn_string& name) const {
        if(!mDocument)
            return false;
        
        pugi::xml_attribute attr = mCurrNode.attribute(name.c_str());
        if(attr) {
            return true;
        }
        return false;
    }
    
    std::vector<ukn_string> ConfigParserXmlImpl::getAttributes() const {
        if(!mDocument)
            return std::vector<ukn_string>();
        
        pugi::xml_attribute_iterator it = mCurrNode.attributes_begin();
        pugi::xml_attribute_iterator end = mCurrNode.attributes_end();
        
        std::vector<ukn_string> attributes;
        for(; it != end; ++it) {
            attributes.push_back(ukn_string(it->value()));
        }
        return attributes;
    }
    
    ukn_string ConfigParserXmlImpl::getString(const ukn_string& attr, const ukn_string& opt) const {
        if(!mDocument)
            return opt;
        
        if(attr.empty()) {
            ukn_string child_value = mCurrNode.child_value();
            return child_value;
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.value();
        } else {
            ukn_string c = mCurrNode.child_value(attr.c_str());
            if(!c.empty())
                return c;
        }
        return opt;
    }
    
    bool ConfigParserXmlImpl::getBool(const ukn_string& attr, bool opt) const {
        if(!mDocument)
            return opt;
        
        if(attr.empty()) {
            return opt;
        }
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_bool();
        }
        return opt;
    }
    
    int32 ConfigParserXmlImpl::getInt(const ukn_string& attr, int32 opt) const {
        if(!mDocument)
            return opt;
        
        if(attr.empty())
            return opt;
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_int();
        }
        return opt;
    }
    
    float ConfigParserXmlImpl::getFloat(const ukn_string& attr, float opt) const {
        if(!mDocument)
            return opt;
        
        if(attr.empty())
            return opt;
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute) {
            return attribute.as_float();
        }
        return opt;
    }
    
    void ConfigParserXmlImpl::myWritter::write(const void* data, size_t size) {
        str += ukn_string((const char*)data, size);
        str += indent;
    }
    
    void ConfigParserXmlImpl::myStreamWritter::write(const void* data, size_t size) {
        stream->write((const uint8*)data, size);
        stream->write((const uint8*)indent.data(), indent.size());
    }
    
    ukn_string ConfigParserXmlImpl::writeToString(const char* indent) const {
        if(!mDocument)
            return ukn_string();
        
        myWritter writter(indent);
        mDocument->save(writter, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_wchar);
        return writter.str;
    }
    
    ConfigParserXmlImpl::myStreamWritter::myStreamWritter(const char* indent) {
        stream = MakeSharedPtr<MemoryStream>();
        this->indent = indent;
    }
    
    ConfigParserXmlImpl::myWritter::myWritter(const char* indent) {
        this->indent = indent;
    }
    
    StreamPtr ConfigParserXmlImpl::writeToStream(const char* indent) const {
        if(!mDocument)
            return MakeSharedPtr<MemoryStream>();
        
        myStreamWritter writter(indent);
        mDocument->save(writter, PUGIXML_TEXT("\t"), pugi::format_indent, pugi::encoding_wchar);
        return writter.stream;
    }
    
    bool ConfigParserXmlImpl::beginNode(const ukn_string& name) {
        if(!mDocument)
            return false;
        
        pugi::xml_node node = mCurrNode.append_child(name.c_str());
        if(node) {
            mCurrNode = node;
            return true;
        }
        return false;
    }
    
    void ConfigParserXmlImpl::endNode() {
        toParent();
    }
    
    void ConfigParserXmlImpl::setValue(const ukn_string& val) {
        if(!mDocument)
            return;
        
        mCurrNode.set_value(val.c_str());
    }
    
    void ConfigParserXmlImpl::setString(const ukn_string& attr, const ukn_string& val) {
        if(!mDocument)
            return;
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val.c_str());
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val.c_str());
        }
    }
    
    void ConfigParserXmlImpl::setBool(const ukn_string& attr, bool val) {
        if(!mDocument)
            return;
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
    
    void ConfigParserXmlImpl::setInt(const ukn_string& attr, int32 val) {
        if(!mDocument)
            return;
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
    
    void ConfigParserXmlImpl::setFloat(const ukn_string& attr, float val) {
        if(!mDocument)
            return;
        
        pugi::xml_attribute attribute = mCurrNode.attribute(attr.c_str());
        if(attribute)
            attribute.set_value(val);
        else {
            attribute = mCurrNode.append_attribute(attr.c_str());
            attribute.set_value(val);
        }
    }
    
    const String& ConfigParserXmlImpl::getName() const {
        return mName;
    }
    
    
} // namespace ukn
