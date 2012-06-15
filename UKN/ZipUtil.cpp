//
//  ZipUtil.cpp
//  Project Unknown
//
//  Created by Ruiwei Bu on 1/10/12.
//  Copyright (c) 2012 heizi. All rights reserved.
//

#include "UKN/ZipUtil.h"
#include "UKN/Stream.h"
#include "UKN/Logger.h"

#include <zlib/unzip.h>

namespace ukn {
    
    static void log_zlib_error(int error) {
        switch (error) {
            case Z_MEM_ERROR:
                log_error("ukn::zlib: Out of memory while (de)compressing data!");
                break;
                
            case Z_VERSION_ERROR:
                log_error("ukn::zlib: Incompatible zlib version!");
                break;
                
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
                log_error("ukn::zlib: Incorrect zlib compressed data!");
                break;
                
            default:
                log_error("ukn::zlib: Unknown error while (de)compressing data!");
        }
    }
    
    Array<uint8> zlib_compress(uint8* input, uint32 input_length, ZlibCompressionMethod method) {

        uint32 buffer_size = 1024;
        Array<uint8> buffer;
        buffer.resize(buffer_size);
        
        int err;
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        strm.next_in  = input;
		strm.avail_in = input_length;
		strm.next_out = buffer.begin();
		strm.avail_out = buffer_size;
        
        const int windowBits = (method == ZCM_Gzip) ? 15 + 16 : 15;
        
        err = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits,
                           8, Z_DEFAULT_STRATEGY);
        if (err != Z_OK) {
            log_zlib_error(err);
            return Array<uint8>();
        }
        
        do {
            err = deflate(&strm, Z_FINISH);
            ukn_assert(err != Z_STREAM_ERROR);
            
            if (err == Z_OK) {
                buffer.resize(buffer_size * 2);

                strm.next_out = (Bytef *)(buffer.begin() + buffer_size);
                strm.avail_out = buffer_size;
                buffer_size *= 2;

            }
        } while (err == Z_OK);
        
        if (err != Z_STREAM_END) {
            log_zlib_error(err);
            deflateEnd(&strm);
            return Array<uint8>();
        }
        
        size_t outLength = buffer_size - strm.avail_out;
        deflateEnd(&strm);
        
        buffer.resize(outLength);

        return buffer;
    }
    
    Array<uint8> zlib_decompress(uint8* input, uint32 input_length) {        
		z_stream strm; /* decompression stream */	
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
        
        uint32 buffer_size = 1024;
        Array<uint8> buffer;
        buffer.resize(buffer_size);
        
		strm.next_in  = input;
		strm.avail_in = input_length;
		strm.next_out = buffer.begin();
		strm.avail_out = buffer_size;
        
		int ret = inflateInit2(&strm, 15 + 32);
        
        if (ret != Z_OK) {
            log_zlib_error(ret);
            return Array<uint8>();
        }
        
        do {
            ret = inflate(&strm, Z_NO_FLUSH);
            
            switch (ret) {
                case Z_NEED_DICT:
                case Z_STREAM_ERROR:
                    ret = Z_DATA_ERROR;
                    
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&strm);
                    log_zlib_error(ret);
                    return Array<uint8>();
            }
            
            if (ret != Z_STREAM_END) {
                buffer.resize(buffer_size * 2);
                
                strm.next_out = (Bytef *)(buffer.begin() + buffer_size);
                strm.avail_out = buffer_size;
                buffer_size *= 2;
            }
        }
        while (ret != Z_STREAM_END);
        
        if (strm.avail_in != 0) {
            log_zlib_error(Z_DATA_ERROR);
            return Array<uint8>();
        }
                        
        size_t outLength = buffer_size - strm.avail_out;
        inflateEnd(&strm);
        
        buffer.resize(outLength);
        
        return buffer;
    }
    
} // namespace ukn
