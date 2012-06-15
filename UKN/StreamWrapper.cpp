//
//  StreamWrapper.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/22/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "UKN/StreamWrapper.h"
#include "UKN/Convert.h"

namespace ukn {
    
    StreamWriter::~StreamWriter() {
        
    }
    
    void StreamWriter::writeLine(uint8 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(char val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(int16 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(uint16 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(int32 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(uint32 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(int64 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(uint64 val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(float val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(double val) {
        this->write(val);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(const uint8* bytes, size_t len) {
        this->write(bytes, len);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(const uint8* bytes, size_t start, size_t end) {
        this->write(bytes, start, end);
        this->writeLineSeparator();
    }
    
    void StreamWriter::writeLine(const ukn_string& str) {
        this->write(str);
        this->writeLineSeparator();
    }
    
    BinaryStreamWriter::BinaryStreamWriter(const StreamPtr& stream):
    mStream(stream) {
        ukn_assert(stream && stream->canWrite());
    }
    
    BinaryStreamWriter::~BinaryStreamWriter() {
        
    }
       
    void BinaryStreamWriter::close() {
        flush();
        mStream->close();
    }
    
    void BinaryStreamWriter::flush() {
        if(mStream.isValid()) {
            mStream->write(mBuffer.begin(), mBuffer.size());
            mBuffer.clear();
        } else {
            UKN_THROW_EXCEPTION("ukn::BinaryStreamWriter::flush: stream not valid, maybe it has been closed before flush?");
        }
    }
    
    void BinaryStreamWriter::write(uint8 val) {
        mBuffer.append(val);
    }
    
    void BinaryStreamWriter::write(char val) {
        mBuffer.append(val);
    }
    
    void BinaryStreamWriter::write(int16 val) {
        mBuffer.append((uint8*)&val, 2);
    }
    
    void BinaryStreamWriter::write(int32 val) {
        mBuffer.append((uint8*)&val, 4);
    }
    
    void BinaryStreamWriter::write(int64 val) {
        mBuffer.append((uint8*)&val, 8);
    }
    
    void BinaryStreamWriter::write(uint16 val) {
        mBuffer.append((uint8*)&val, 2);
    }
    
    void BinaryStreamWriter::write(uint32 val) {
        mBuffer.append((uint8*)&val, 4);
    }
    
    void BinaryStreamWriter::write(uint64 val) {
        mBuffer.append((uint8*)&val, 8);
    }
    
    void BinaryStreamWriter::write(float val) {
        mBuffer.append((uint8*)&val, sizeof(float));
    }
    
    void BinaryStreamWriter::write(double val) {
        mBuffer.append((uint8*)&val, sizeof(double));
    }
    
    void BinaryStreamWriter::write(const uint8* bytes, size_t len) {
        mBuffer.append(bytes, len);
    }
    
    void BinaryStreamWriter::write(const uint8* bytes, size_t start, size_t end) {
        mBuffer.append(bytes + start, end - start);
    }
    
    void BinaryStreamWriter::write(const ukn_string& str) {
        mBuffer.append((uint8*)str.c_str(), str.length()+1);
    }
    
    void BinaryStreamWriter::writeLineSeparator() {
        mBuffer.append('\n');
    }
    
    BinaryStreamReader::BinaryStreamReader(const StreamPtr& stream):
    mStream(stream) {
        ukn_assert(stream && stream->canWrite());
    }
    
    TextStreamWriter::TextStreamWriter(const StreamPtr& stream):
    mStream(stream) {
        
    }
    
    TextStreamWriter::~TextStreamWriter() {
        
    }
    
    void TextStreamWriter::flush() {
        if(mStream)
            mStream->flush();
    }
    
    void TextStreamWriter::close() {
        if(mStream)
            mStream->close();
    }
    
    void TextStreamWriter::write(uint8 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(char val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(int16 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(uint16 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(int32 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(uint32 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(int64 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(uint64 val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(float val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(double val) {
        if(mStream && mStream->isValid())
            this->write(String::AnyToString(val));
    }
    
    void TextStreamWriter::write(const uint8* bytes, size_t len) {
        if(mStream && mStream->isValid())
            this->write(ukn_string((const char*)bytes, len));
    }
    
    void TextStreamWriter::write(const uint8* bytes, size_t start, size_t end) {
        if(mStream && mStream->isValid())
            this->write(ukn_string((const char*)bytes, start, end));
    }
    
    void TextStreamWriter::write(const ukn_string& str) {
        if(mStream && mStream->isValid()) 
            mStream->write((const uint8*)str.c_str(), str.length());
    }
    
    void TextStreamWriter::writeLineSeparator() {
        if(mStream && mStream->isValid()) {
            static uint8 sp = '\n';
            mStream->write(&sp, 1);
        }
    }
    
    StreamReader::~StreamReader() {
        
    }
    
    BinaryStreamReader::~BinaryStreamReader() {
        
    }
    
    void BinaryStreamReader::close() {
        mStream->close();
    }
    
    char BinaryStreamReader::peek() {
        if(!mStream->isValid()) 
            return -1;
        
        size_t pos = mStream->pos();
        if(pos == mStream->size())
            return -1;
        
        char buffer;
        mStream->read((uint8*)&buffer, 1);
        mStream->seek(pos);
        return buffer;
    }
    
    size_t BinaryStreamReader::read(uint8* buffer, size_t size_to_read) {
        return mStream->read(buffer, size_to_read);
    }
    
    uint8 BinaryStreamReader::readByte() {
        if(mStream && mStream->isValid()) { 
            uint8 buffer;
            mStream->read(&buffer, 1);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    uint16 BinaryStreamReader::readUInt16() {
        if(mStream && mStream->isValid()) { 
            uint16 buffer;
            mStream->read((uint8*)&buffer, 2);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    uint32 BinaryStreamReader::readUInt32() {
        if(mStream && mStream->isValid()) { 
            uint32 buffer;
            mStream->read((uint8*)&buffer, 4);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    uint64 BinaryStreamReader::readUInt64() {
        if(mStream && mStream->isValid()) { 
            uint64 buffer;
            mStream->read((uint8*)&buffer, 8);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    int8 BinaryStreamReader::readChar() {
        if(mStream && mStream->isValid()) { 
            int8 buffer;
            mStream->read((uint8*)&buffer, 1);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    int16 BinaryStreamReader::readInt16() {
        if(mStream && mStream->isValid()) { 
            int16 buffer;
            mStream->read((uint8*)&buffer, 2);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    int32 BinaryStreamReader::readInt32() {
        if(mStream && mStream->isValid()) { 
            int32 buffer;
            mStream->read((uint8*)&buffer, 4);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    int64 BinaryStreamReader::readInt64() {
        if(mStream && mStream->isValid()) { 
            int64 buffer;
            mStream->read((uint8*)&buffer, 8);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    float BinaryStreamReader::readFloat() {
        if(mStream && mStream->isValid()) { 
            float buffer;
            mStream->read((uint8*)&buffer, 4);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    double BinaryStreamReader::readDouble() {
        if(mStream && mStream->isValid()) { 
            double buffer;
            mStream->read((uint8*)&buffer, 8);
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    ukn_string BinaryStreamReader::readString() {
        if(mStream && mStream->isValid()) { 
            ukn_string buffer;
            uint8 c;
            mStream->read(&c, 1);
            while(c != 0 && !mStream->eos()) {
                buffer.push_back((char)c);
                mStream->read(&c, 1);
            }
            return buffer;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    ukn_string BinaryStreamReader::readString(size_t char_count, StringFormat encoding_format) {
        if(mStream && mStream->isValid()) { 
            uint8* char_data = ukn_malloc_t(uint8, char_count);
            size_t read_count = mStream->read(char_data, char_count);
            if(read_count != char_count) {
            // exception
            }
            ukn_string str((const char*)char_data, encoding_format);
            ukn_free(char_data);
            return str;
        }
        UKN_THROW_EXCEPTION("ukn::BinaryStreamReader::read: invalid stream, maybe it's the end");
    }
    
    TextStreamReader::TextStreamReader(const StreamPtr& stream):
    mStream(stream) {
        
    }
    
    TextStreamReader::~TextStreamReader() {
        
    }
    
    void TextStreamReader::close() {
        if(mStream)
            mStream->close();
    }
    
    char TextStreamReader::peek() {
        if(!mStream->isValid()) 
            return -1;
        
        size_t pos = mStream->pos();
        if(pos == mStream->size())
            return -1;
        
        char buffer;
        mStream->read((uint8*)&buffer, 1);
        mStream->seek(pos);
        return buffer;
    }
    
    size_t TextStreamReader::read(uint8* buffer, size_t size_to_read) {
        return mStream->read(buffer, size_to_read);
    }
    
    uint8 TextStreamReader::readByte() {
        return Convert::ToByte(this->readNumber());
    }
    
    uint16 TextStreamReader::readUInt16() {
        return Convert::ToUInt16(this->readNumber());
    }
    
    uint32 TextStreamReader::readUInt32() {
        return Convert::ToUInt32(this->readNumber());
    }
    
    uint64 TextStreamReader::readUInt64() {
        return Convert::ToUInt64(this->readNumber());
    }
    
    int8 TextStreamReader::readChar() {
        return Convert::ToByte(this->readNumber());
    }
    
    int16 TextStreamReader::readInt16() {
        return Convert::ToInt16(this->readNumber());
    }
    
    int32 TextStreamReader::readInt32() {
        return Convert::ToInt32(this->readNumber());
    }
    
    int64 TextStreamReader::readInt64() {
        return Convert::ToUInt64(this->readNumber());
    }
    
    float TextStreamReader::readFloat() {
        return Convert::ToDouble(this->readNumber());
    }
    
    double TextStreamReader::readDouble() {
        return Convert::ToDouble(this->readNumber());
    }
    
    inline bool is_space(char c) {
        return c == ' ' || 
               c == '\n' ||
               c == '\r';
    }
    
    ukn_string TextStreamReader::readString() {
        ukn_string result;
        
        uint8 buffer = ' ';
        while(is_space((char)buffer))  
            mStream->read(&buffer, 1);
              
        while(mStream->isValid() &&
              !is_space((char)buffer)) {
            result.push_back((char)buffer);
            mStream->read(&buffer, 1);
        }
        return result;
    }
    
    ukn_string TextStreamReader::readLine() {
        return readTill('\n');
    }
    
    ukn_string TextStreamReader::readTill(char end) {
        ukn_string result;
        
        uint8 buffer;
        mStream->read(&buffer, 1);
        while(!mStream->eos() &&
              ((char)buffer) != end) {
            result.push_back((char)buffer);
            mStream->read(&buffer, 1);
        }
        return result;
    }
    
    ukn_string TextStreamReader::readNumber() {
        ukn_string result;
        
        uint8 buffer = ' ';
        while(is_space((char)buffer))  
            mStream->read(&buffer, 1);
        
        while(!mStream->eos() &&
              (((char)buffer >= '0' && (char)buffer <= '9') || (char)buffer == '.')) {
            result.push_back((char)buffer);
            mStream->read(&buffer, 1);
        }
        return result;
    }
    
} // namespace ukn

