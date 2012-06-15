//
//  Stream.h
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Stream_h
#define Project_Unknown_Stream_h

#include "UKN/Platform.h"
#include "UKN/Util.h"
#include "UKN/PreDeclare.h"
#include "UKN/StringUtil.h"

namespace ukn {
    
    enum StreamType {
        ST_Memory,
        ST_File,
        ST_Net,
    };
    
    class UKN_API Stream {
    public:
        virtual ~Stream() { }
        
    	virtual bool	canRead() const = 0;
    	virtual bool	canWrite() const = 0;
    	virtual bool 	canSeek() const = 0;
    	
        virtual bool    seek(size_t pos) = 0;
        virtual size_t  read(uint8* buffer, size_t length) = 0;
        virtual size_t  write(const uint8* buffer, size_t length) = 0;
        
        virtual size_t  pos() const = 0;
        virtual size_t  size() const = 0;
        virtual bool    isValid() const = 0;
        
        virtual void    close() = 0;
        virtual void    flush() = 0;
        virtual bool    eos() const = 0;
        
        virtual StreamType getStreamType() const = 0;
        
        virtual uint8       readByte();
        virtual void        writeByte(uint8 byte);
        
        virtual bool        saveToFile(const String& file);
        virtual StreamPtr   readIntoMemory();
    };

    class UKN_API MemoryStream: public Stream {
	public:
		MemoryStream();
        MemoryStream(const MemoryStream& rhs);
		MemoryStream(const uint8* data, size_t length);
		
		virtual ~MemoryStream();
        
        MemoryStream& operator=(const MemoryStream& rhs);
        
        void close() override;
        void flush() override;
        
        StreamType getStreamType() const override;
        
        // give memory management to MemoryStream
		void    set(uint8* data, size_t length);
        // only holds the data pointer, does not do memory management
        void    map(uint8* data, size_t length);
        size_t  write(const uint8* data, size_t length) override;

        bool    seek(size_t pos) override;
        bool    eos() const override;
		
		size_t  size() const override;
		size_t  capacity() const;
		size_t  pos() const override;
		bool    isValid() const override;
        
		bool alloc(size_t size);
		
		void resize();
        void resize(size_t newsize);
		        
		template<typename T>
		T read();
        
		size_t read(uint8* pv, size_t size) override;		
		size_t read(size_t offset, uint8* pv, size_t size);
		
        uint8* data();
        uint8* dataOf(size_t offset);
		const uint8* data() const;
		const uint8* dataOf(size_t offset) const;
		
		template<typename datatype> 
        datatype& get(size_t offset) const { 
            if(offset > this->size()+sizeof(datatype)) offset = 0;
            return *(datatype*)(mData[offset]);
        }
        
        uint8 operator[](size_t index);
        
        bool canRead() const override;
        bool canWrite() const override;
        bool canSeek() const override;
        
        StreamPtr readIntoMemory() override;
        
	private:
		size_t mCurrPos;
        Array<uint8> mData;
	};
    
    template<typename T>
    T MemoryStream::read() {
        if(!isValid()) return 0;
        if(this->mCurrPos == this->size()) return 0;
        
        uint32 size = sizeof(T);
        if(this->mCurrPos + size <= this->size()) {
            T t;
            memcpy(&t, (void*)(data()+this->mCurrPos), size);
            this->mCurrPos += size;
            return t;
        } else 
            UKN_THROW_EXCEPTION("ukn::MemoryStream::read<T>: stream length not enough");
        return 0;
    }
    
    /*
     POSIX implemention
     */
    class UKN_API FileStream: public Stream {
    public:
        enum SeekType {
            Begin,
            Pos,
            End
        };
        
    public:
        
        FileStream();
        virtual ~FileStream();
        virtual bool open(const String& filename, bool canwrite = false, bool append = false, bool nocache = false);
        
        bool canRead() const override;
        bool canWrite() const override;
        bool canSeek() const override;
        
        void close() override;
        void flush() override;

        void truncate();
        size_t size() const override;
        size_t pos() const override;
        bool eos() const override;
        
        bool isValid() const override;
        bool seek(size_t pos) override;
        bool seek(size_t pos, SeekType type);
        size_t read(uint8* data, size_t len) override;
        size_t write(const uint8* data, size_t len) override;
        
        StreamType getStreamType() const override;
        StreamPtr readIntoMemory() override;
        
    private:
        bool nocache;
        bool canwrite;
        FILE* file;
    };
    
    class BufferedStream: public Stream {
    public:
        BufferedStream(const StreamPtr& stream);
        BufferedStream(const StreamPtr& stream, size_t buffer_size);
        virtual ~BufferedStream();
        
        bool	canRead() const override;
        bool	canWrite() const override;
        bool 	canSeek() const override;
    	
        bool    seek(size_t pos) override;
        size_t  read(uint8* buffer, size_t length) override;
        size_t  write(const uint8* buffer, size_t length) override;
        
        bool    eos() const override;
        size_t  pos() const override;
        size_t  size() const override;
        bool    isValid() const override;
        
        void    close() override;
        void    flush() override;
        
        StreamType getStreamType() const override;
        StreamPtr readIntoMemory() override;
        
    private:
        Array<uint8> mBuffer;
        StreamPtr mStream;
    };
        
    /**
     * simple stream that receives / sends data through internet
     * some stream actions may not available for net stream
     * such as size and pos
     * implementation from http://www.keithschwarz.com/interesting/code/?dir=nstream by Keith Schwarz
     **/
    class NetStream: public Stream, public std::iostream {
    public:
        NetStream();
        explicit NetStream(const ukn_string& hostName, uint16 portNum);
        explicit NetStream(uint16 port);

        ~NetStream();
        
        bool canRead() const;
        bool canWrite() const;
        bool canSeek() const;
        
        // wait for a connection
        // port no. 0-32767
        void open(uint16 port);
        
        // connect to a server
        void open(const ukn_string& server, uint16 port);
        
        bool is_open() const;
        
        void close();
        void flush();
        
        bool    seek(size_t pos);
        size_t  read(uint8* buffer, size_t length);
        size_t  write(const uint8* buffer, size_t length);
        
        size_t  pos() const;
        size_t  size() const;
        bool    isValid() const;
        
    private:
        class SocketStreambuf;
        
        SocketStreambuf* connection;
    };
    
} // namespace ukn


#endif
