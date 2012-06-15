//
//  Stream.cpp
//  Project Unknown
//
//  Created by Robert Bu on 11/23/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#include "UKN/Stream.h"
#include "UKN/MemoryUtil.h"
#include "UKN/StringUtil.h"
#include "UKN/Logger.h"

namespace ukn {
    
    uint8 Stream::readByte() {
        if(!canRead()) {
            UKN_THROW_EXCEPTION("ukn::Stream: cannot read byte because the stream cannot read");
        }
        uint8 buffer;
        this->read(&buffer, 1);
        return buffer;
    }
    
    void Stream::writeByte(uint8 byte) {
        if(!canRead()) {
            UKN_THROW_EXCEPTION("ukn::Stream: cannot write byte because the stream cannot write");
        }
        this->write(&byte, 1);
    }
    
    bool Stream::saveToFile(const String& file) {
        FileStream output;
        if(output.open(file, true)) {
            if(this->getStreamType() == ST_Memory) {
                output.write(static_cast<MemoryStream*>(this)->data(), this->size());
            } else if(this->getStreamType() == ST_File) {
                uint8* buffer = ukn_malloc_t(uint8, this->size());
                if(buffer) {
                    size_t readSize = this->read(buffer, this->size());
                    output.write(buffer, readSize);
                    
                    ukn_free(buffer);
                }
            }
            output.close();
            return true;
        }
        return false;
    }
    
    StreamPtr Stream::readIntoMemory() {
        if(isValid()) {
            uint8* dataBuffer = (uint8*)ukn_malloc(size());
            ukn_assert(dataBuffer != 0);
            
            this->seek(0);
            size_t readSize = this->read(dataBuffer, size());
            if(readSize != size()) {
                // log error
                log_error("ukn::Stream::readIntoMemory: error happened when trying to read");
            }
            
            MemoryStream* memBuffer = new MemoryStream();
            // give memory management to MemoryStream
            memBuffer->set(dataBuffer, readSize);
            
            this->seek(0);
            
            return StreamPtr(memBuffer);
        } else 
            // return null memorystream
            return MakeSharedPtr<MemoryStream>();
    }
 
    MemoryStream::MemoryStream():  
    mCurrPos(0) {
    }
    
    MemoryStream::MemoryStream(const MemoryStream& rhs):
    mCurrPos(0),
    mData(rhs.mData) {

    }
    
	MemoryStream::MemoryStream(const uint8* data, size_t length):
	mCurrPos(0) {
	}
	
	MemoryStream::~MemoryStream() {
        
    }
    
    MemoryStream& MemoryStream::operator=(const MemoryStream& rhs) {
        if(this != &rhs) {
            this->mCurrPos = rhs.mCurrPos;
            this->mData = rhs.mData;
        }
        return *this;
    }
    
    bool MemoryStream::canRead() const {
    	return true;
    }
    
	bool MemoryStream::canWrite() const {
		return true;
	}
	
    bool MemoryStream::canSeek() const {
    	return true;
    }
    
    void MemoryStream::close() {
        this->mData.clear();
    }
    
    void MemoryStream::flush() {
        
    }
    
    StreamType MemoryStream::getStreamType() const {
        return ST_Memory;
    }
    
    uint8 MemoryStream::operator[](size_t index) {
        ukn_assert(index < this->size());
        
        return mData[index];
    }
	
	void MemoryStream::set(uint8* data, size_t length) {
         mData.set(data, length);
        mCurrPos = 0;
	}
	
    void MemoryStream::map(uint8* data, size_t length) {        
         mData.map(data, length);
        mCurrPos = 0;
    }
    
	bool MemoryStream::alloc(size_t size) {
		 mData.realloc(size, 0);
        
		seek(0);
		return true;
	}
	
	void MemoryStream::resize() {
		if(size() != capacity() && capacity() != 0) {
			uint8* tmpData = (uint8*)ukn_malloc(size());
            seek(0);
			memcpy(tmpData, (void*)(data()), size());
			
            mData.set(tmpData, size());
            
            ukn_free(tmpData);
		}
	}
        
    void MemoryStream::resize(size_t newsize) {
         mData.realloc(newsize, 0);
    }
	
	size_t MemoryStream::write(const uint8* data, size_t length) {
        mData.append(data, length);
		return length;
	}
	
	size_t MemoryStream::read(uint8* pv, size_t length) {
		if(!isValid()) return 0;
		if(mCurrPos == size()) return 0;
		
		assert(pv != NULL);
		
		if(mCurrPos+length > size()) {
			length = size() - mCurrPos;
		}
		memcpy(pv, (void*)(data()+mCurrPos), length);
		mCurrPos += length;
		return length;
	}
	
	size_t MemoryStream::read(size_t offset, uint8* pv, size_t length) {
		if(!isValid()) return false;
		
		size_t tempPos = mCurrPos;
		if(!seek(offset))
			seek(0);
		size_t result = read(pv, length);
		seek(tempPos);
		return result;
	}
	
	const uint8* MemoryStream::data() const { 
        return mData.begin();
    }
    
	const uint8* MemoryStream::dataOf(size_t offset) const { 
		if(offset > size()) 
            offset = 0;
		return ( mData.begin()+offset);
	}
	
	bool MemoryStream::seek(size_t pos) {
		if(pos <= size()) {
			mCurrPos = pos;
			return true;
		}
		return false;
	}
	
	size_t MemoryStream::size() const { 
        return mData.size();
    }
    
	size_t MemoryStream::capacity() const { 
        return mData.capacity();
    }
    
	size_t MemoryStream::pos() const { 
        return mCurrPos; 
    }
    
    uint8* MemoryStream::data() {
        return mData.begin();
    }
    
    uint8* MemoryStream::dataOf(size_t offset) {
        if(offset > size()) 
            offset = 0;
		return ( mData.begin()+offset);
    }
    
	bool MemoryStream::isValid() const { 
        return !mData.empty();
    }
    
    StreamPtr MemoryStream::readIntoMemory() {
        return new MemoryStream(*this);
    }
    
    bool MemoryStream::eos() const {
        return this->mCurrPos == mData.size();
    }
    
    /*
#ifdef UKN_OS_WINDOWS
    
    FileStreamWin32::FileStreamWin32() {
        file = NULL;
    }
    
    FileStreamWin32::~FileStreamWin32() {
        close();
    }
    
    bool FileStreamWin32::open(const String& filename, bool canwrite, bool append, bool nocache) {
        if(file)
            return false;
            
        this->canwrite = canwrite;
        
        file = CreateFileW(filename.c_str(), 
                           (canwrite ? GENERIC_WRITE : 0) | GENERIC_READ, FILE_SHARE_READ, NULL, canwrite ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | (nocache ? FILE_FLAG_WRITE_THROUGH : 0), 
                           NULL);
        if(file == NULL || file == INVALID_HANDLE_VALUE) {
            return false;
        }
        if(append) {
            SetFilePointer(file, 0, NULL, FILE_END);
        }
		return true;
    }
    
    bool FileStreamWin32::canRead() const {
    	return true;
    }
    
	bool FileStreamWin32::canWrite() const {
		return canwrite;
	}
	
    bool FileStreamWin32::canSeek() const {
    	return true;
    }
    
    void FileStreamWin32::close() {
        if(file != NULL) {
            CloseHandle(file);
            file = NULL;
        }
    }
    
    void FileStreamWin32::flush() {
        
    }
    
    void FileStreamWin32::truncate() {
        SetEndOfFile(file);
    }
    
    size_t FileStreamWin32::size() const {
        DWORD low, high;
        low = GetFileSize(file, &high);
        return (uint64)low + (((uint64)high)<<32);
    }
    
    size_t FileStreamWin32::pos() const {
        DWORD low;
        LONG high = 0;
        low = SetFilePointer(file, 0, &high, FILE_CURRENT);
        return (uint64)low + ((high)<<32);
    }
    
    bool FileStreamWin32::seek(size_t pos, SeekType type) {
        LONG high = (LONG)(pos >> 32);
        return SetFilePointer(file, pos & 0xFFFFFFFF, (high > 0 ? &high : NULL), type) != INVALID_SET_FILE_POINTER;
    }
    
    bool FileStreamWin32::isValid() const {
        return file != 0;
    }
    
    bool FileStreamWin32::seek(size_t pos) {
        LONG high = (LONG)(pos >> 32);
        return SetFilePointer(file, pos & 0xFFFFFFFF, (high > 0 ? &high : NULL), Current) != INVALID_SET_FILE_POINTER;
    }
    
    size_t FileStreamWin32::read(uint8* data, size_t len) {
        DWORD size;
        if(ReadFile(file, data, len, &size, NULL))
            return size;
        return 0;
    }
    
    size_t FileStreamWin32::write(const uint8* data, size_t len) {
        DWORD size;
        if(WriteFile(file, data, len, &size, NULL))
            return size;
        return 0;
    }
    
#else 
    */
    FileStream::FileStream() {
        nocache = false;
        file = NULL;
    }
    
    FileStream::~FileStream() {
        close();
    }
    
    bool FileStream::canRead() const {
    	return true;
    }
    
	bool FileStream::canWrite() const {
		return canwrite;
	}
	
    bool FileStream::canSeek() const {
    	return true;
    }
    
    bool FileStream::eos() const {
        if(file)
            return feof(file);
        return true;
    }
    
    bool FileStream::open(const String& filename, bool canwrite, bool append, bool nocache) {
        if(file != NULL)
            return false;
        
        this->nocache = nocache;
        this->canwrite = canwrite;
        file = fopen(String::WStringToString(filename).c_str(), canwrite ? (append ? "a+b" : "r+b") : "rb");
        if(file == NULL) {
            if(canwrite)
                file = fopen(String::WStringToString(filename).c_str(), "w+b");
            if(file == NULL)
                return false;
        }
        return true;
    }
    
    void FileStream::close() {
        if(file != NULL) {
            fclose(file);
            file = NULL;
        }
    }
    
    void FileStream::truncate() {
        
    }
    
    void FileStream::flush() {
        if(file != 0)
            fflush(file);
    }
    
    size_t FileStream::size() const {
        size_t pos = ftell(file);
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, pos, SEEK_SET);
        return size;
    }
    
    size_t FileStream::pos() const {
        if(file != 0)
            return ftell(file);
        return 0;
    }
    
    bool FileStream::isValid() const {
        return file != 0;
    }
    
    bool FileStream::seek(size_t pos) {
        if(file != 0)
            return fseek(file, pos, SEEK_SET) == 0 ? true : false;
        return false;
    }
    
    bool FileStream::seek(size_t pos, SeekType type) {
        if(file != 0)
            return fseek(file, pos, type) == 0 ? true : false;
        return false;
    }
    
    size_t FileStream::read(uint8* data, size_t len) {
        if(file != 0)
            return fread(data, 1, len, file);
        return 0;
    }
    
    size_t FileStream::write(const uint8* data, size_t len) {
        if(file == 0)
            return 0;
        
        if(!nocache) 
            return fwrite(data, len, 1, file);
        else {
            size_t size = fwrite(data, len, 1, file);
            fflush(file);
            return size;
        }
    }
    
    StreamType FileStream::getStreamType() const {
        return ST_File;
    }
    
    StreamPtr FileStream::readIntoMemory() {
        if(isValid()) {
            uint8* dataBuffer = (uint8*)ukn_malloc(size());
            ukn_assert(dataBuffer != 0);
            
            this->seek(0);
            size_t readSize = this->read(dataBuffer, size());
            if(readSize != size()) {
                // log error
            }
            
            MemoryStream* memBuffer = new MemoryStream();
            // give memory management to MemoryStream
            // faster
            memBuffer->set(dataBuffer, readSize);
            
            this->seek(0);
            
            return StreamPtr(memBuffer);
        } else 
            // return null memorystream
            return MakeSharedPtr<MemoryStream>();
    }
    
    BufferedStream::BufferedStream(const StreamPtr& stream):
    mStream(stream) {
        
    }
    
    BufferedStream::BufferedStream(const StreamPtr& stream, size_t buffer_size):
    mStream(stream),
    mBuffer(buffer_size, 0) {
        
    }
    
    BufferedStream::~BufferedStream() {
        
    }
    
    bool BufferedStream::eos() const {
        return mStream->eos();
    }
    
    bool BufferedStream::canRead() const {
        return mStream->canRead();
    }
    
    bool BufferedStream::canWrite() const {
        return mStream->canWrite();
    }
    
    bool BufferedStream::canSeek() const {
        return mStream->canSeek();
    }
    
    bool BufferedStream::seek(size_t pos) {
        return mStream->seek(pos);
    }
    
    size_t BufferedStream::read(uint8* buffer, size_t length) {
        return mStream->read(buffer, length);
    }
    
    size_t BufferedStream::write(const uint8* buffer, size_t length) {
        mBuffer.append(buffer, length);
        return length;
    }
    
    size_t BufferedStream::pos() const {
        return mStream->pos();
    }
    
    size_t BufferedStream::size() const {
        return mStream->size();
    }
    
    bool BufferedStream::isValid() const {
        return mStream->isValid();
    }
    
    void BufferedStream::close() {
        mStream->close();
    }
    
    void BufferedStream::flush() {
        if(mStream->isValid()) {
            mStream->write(mBuffer.begin(), mBuffer.size());
            mBuffer.clear();
            mStream->flush();
        } else 
            UKN_THROW_EXCEPTION("ukn::BufferedStream::flush: attempted to write a invalid stream, maybe the stream had been closed?");
    }
    
    StreamType BufferedStream::getStreamType() const {
        return mStream->getStreamType();
    }
    
    StreamPtr BufferedStream::readIntoMemory() {
        return mStream->readIntoMemory();
    }
    
} // namespace ukn


#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>
using namespace std;

/* Utility function to convert an integer into a string. */
static string ShortToString(short value) {
    stringstream converter;
    converter << value;
    return converter.str();
}

#ifdef UKN_OS_WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "WS2_32.lib")

typedef SOCKET SocketType;
typedef int SocketLengthType;
const SocketType NotASocket = INVALID_SOCKET;

static void CloseSocket(SocketType socket) {
    closesocket(socket);
}

/* The socket APIs for Windows and Unix/Mac are similar in some ways, but one
 * major difference is that the Windows API requires some calls to functions at
 * program startup and shutdown.  This handles those cases.
 */
static class WinsockInitializer {
public:
    WinsockInitializer() {
        WSADATA wsaData;
        if(WSAStartup(MAKEWORD(2, 0), &wsaData ) != 0)
            throw runtime_error("Couldn't initialize WINSOCK.");
        
        if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
            throw runtime_error("Incorrect WINSOCK version.");
    }
    
    ~WinsockInitializer() {
        WSACleanup();
    }
} wsi;

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef int SocketType;
typedef socklen_t SocketLengthType;
const SocketType NotASocket = SocketType(-1);

static void CloseSocket(SocketType socket) {
    close(socket);
}

#endif

namespace ukn {
    
    
    class NetStream::SocketStreambuf: public streambuf {
    public:
        /* Constructor that opens a connection to a remote server. */
        SocketStreambuf(const string& filename, const short portNum);
        
        /* Constructor that waits for an incoming connection from a foreign client. */
        explicit SocketStreambuf(const short portNum);
        
        /* Constructor that puts us into an empty state. */
        SocketStreambuf();
        
        /* Dtor cleans everything up. */
        ~SocketStreambuf();
        
        /* Returns whether things are happy on this end. */
        bool isGood() const {
            return good;
        }
        
        /* Returns whether a connection is open. */
        bool isOpen() const {
            return sock != NotASocket;
        }
        
        /* Two versions of open. */
        bool open(const short portNum);
        bool open(const string& hostName, const short portNum);
        
        /* Closes the socket connection, but first flushses any existing data.
         * If not open, reports an error.
         */
        bool close();
        
    protected:
        /* Useful typedefs. */
        typedef traits_type::int_type int_type;
        typedef traits_type::pos_type pos_type;
        typedef traits_type::off_type off_type;
        
        /* On underflow, tries to read in more from the socket and fill things in. */
        virtual int_type underflow();
        
        /* On overflow, send everything we've got. */
        virtual int_type overflow(int_type value = traits_type::eof());
        
        /* Synchronizes the streams by flushing. */
        virtual int sync();
        
        /* Ungets a character.*/
        virtual int_type pbackfail(int_type ch);
        
    private:
        
        /* Shuts down the connection and resets all fields to their initial state. */
        void shutdownConnection();
        
        /* The actual socket. */
        SocketType sock;
        
        /* Nice helper flag that tracks whether or not we're in a good state.
         * If not in a good state, we automatically fail every time we try to
         * read anything.
         */
        bool good;
        
        /* The actual buffer. */
        static const size_t kBufferSize = 4096;
        char readBuffer[kBufferSize];
        char writeBuffer[kBufferSize];
    };
    
    /* Ctor 1 is used to connect to a remote host. */
    NetStream::SocketStreambuf::SocketStreambuf(const string& hostname, const short portNum) :
    sock(NotASocket), good(false) {
        /* Try to set up the socket and fail if unable to do so. */
        open(hostname, portNum);
    }
    
    /* Ctor 2 is used to wait for an incoming connection. */
    NetStream::SocketStreambuf::SocketStreambuf(const short portNum) : sock(NotASocket), good(false) {
        /* Try to set up the connection and fail if unable to do so. */
        open(portNum);
    }
    
    /* Ctor 3 just puts us into an empty state. */
    NetStream::SocketStreambuf::SocketStreambuf() : sock(NotASocket), good(false) {
    }
    
    /* Destructor cleans up the socket. */
    NetStream::SocketStreambuf::~SocketStreambuf() {
        close();
    }
    
    void NetStream::SocketStreambuf::shutdownConnection() {
        /* Time to close things, if we indeed have things to close. */
        if(sock != NotASocket)
            CloseSocket(sock);
        sock = NotASocket;
        good = false;
    }
    
    /* To close a connection, call sync and then reset things. */
    bool NetStream::SocketStreambuf::close() {
        if(!isOpen()) return false;
        
        sync();
        shutdownConnection();
        return true;
    }
    
    /* Try connecting on the specified address/port combination. */
    bool NetStream::SocketStreambuf::open(const string& address, const short portNum) {
        try {
            /* Now, try to look up the target.  Begin by setting up hints accordingly. */
            addrinfo hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            
            /* Hold the result of the getaddrinfo that listens on the indicated port. */
            addrinfo* addresses = NULL;
            const int result = getaddrinfo(address.c_str(), 
                                           ShortToString(portNum).c_str(), 
                                           &hints, &addresses);
            if(result != 0)
                throw runtime_error("Couldn't get address info for this server.");
            
            try  {
                /* Make sure they exist! */
                if(addresses == NULL)
                    throw runtime_error("Got server info, but there were no nodes.");
                
                /* Begin by initializing the socket. */
                sock = socket(addresses->ai_family, addresses->ai_socktype, addresses->ai_protocol);
                if(sock == -1)
                    throw runtime_error("Couldn't create local socket.");
                
                /* Now for the fun part - actually connect! */
                if(connect(sock, addresses->ai_addr, (int) addresses->ai_addrlen) == -1)
                    throw runtime_error("Unable to connect to server.");
                
                /* Once done, make sure to clean things up! */
                freeaddrinfo(addresses);
                
                /* Yay!  We're good. */
                good = true;
            } catch(const runtime_error &){
                /* On error, reclaim resources. */
                if(addresses)
                    freeaddrinfo(addresses);
                if(sock != NotASocket)
                    CloseSocket(sock);
                throw;
            }
        } catch(const runtime_error &) {
            sock = NotASocket;
            return false;
        }
        return true;
    }
    
    /* Try waiting for an incoming connection. */
    bool NetStream::SocketStreambuf::open(const short portNum) {
        /* Begin by setting up information about our machine. */
        addrinfo hints, *addresses = NULL;
        memset(&hints, 0, sizeof(addrinfo));
        
        /* Tell us to allow connections to any IP address on any protocol. */
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        
        /* Call getaddrinfo to learn where to wait. */
        if(getaddrinfo(NULL, ShortToString(portNum).c_str(), &hints, &addresses) != 0)
            return false;
        
        try {
            /* Set up a socket that just sits around waiting for incoming connections.  If
             * at any point we fail, abort.
             */
            SocketType listenerSocket = NotASocket;
            try {
                /* Get a socket to listen with.  Fail if we can't. */
                listenerSocket = socket(addresses->ai_family,
                                        addresses->ai_socktype,
                                        addresses->ai_protocol);
                if(listenerSocket == NotASocket)
                    throw runtime_error("Couldn't create a listener socket.");
                
                /* Bind the socket on the proper port so we can listen.  Fail if we can't. */
                if(::bind(listenerSocket, addresses->ai_addr, (int) addresses->ai_addrlen) != 0)
                    throw runtime_error("Couldn't bind the socket.");
                
                /* Now, tell the socket to listen in for at most one connection. */
                if(::listen(listenerSocket, 1) == -1)
                    throw runtime_error("Couldn't listen for incoming connections.");
                
                /* Finally, set our socket to be the one we get when we find someone who
                 * wants to talk to us.
                 */
                SocketLengthType addrSize = sizeof (sockaddr_storage);
                sockaddr_storage theirAddr;
                sock = accept(listenerSocket, (struct sockaddr *)&theirAddr, &addrSize);
                if(sock == NotASocket)
                    throw runtime_error("Failed to accept a connection.");
                
                /* Yay!  We're listening in!  Now close the listener socket and set us status
                 * to "success."
                 */
                CloseSocket(listenerSocket);
                good = true;
            } catch(const runtime_error &){
                /* Whoops!  Something went wrong.  Clean up the socket and keep unwinding
                 * to execute other cleanup.
                 */
                if(listenerSocket != NotASocket)
                    CloseSocket(listenerSocket);
                throw;
            }
            
            /* Finally, clean up the address info data. */
            freeaddrinfo(addresses);
        } catch(const runtime_error &) {
            /* Remember to clean up the address info.  This is why C++ > C.
             * Use your destructors, kids.
             */
            if(addresses)
                freeaddrinfo(addresses);
            sock = NotASocket;
            return false;
        }
        return true;
    }
    
    /* On an overflow, we need to send all of the data we have. */
    NetStream::SocketStreambuf::int_type
    NetStream::SocketStreambuf::overflow(SocketStreambuf::int_type value)
    {
        /* First, if things are bad, fail immediately. */
        if(!good)
            return traits_type::eof();
        
        /* Continuously send data until we've exhausted the buffer.  Since
         * the socket might not send everything all at once, loop until we
         * have successfully transmitted everything.
         */
        const char* readPtr = pbase();
        while(readPtr != pptr()) {
            /* Send what we can. */
            const int sendResult = (const int)send(sock, readPtr, (size_t)(pptr() - readPtr), 0);
            if(sendResult == -1){
                /* Ouch!  We broke.  How sad.  Now we'll set our put area to
                 * empty and go into a fail state.
                 */
                setp(NULL, NULL);
                good = false;
                return traits_type::eof();
            }
            
            /* Advance forward for the next write. */
            readPtr += sendResult;
        }
        
        /* Now, at the end, we've successfully transmitted.  We now need to
         * reset the put area.  Also, if the char we got wasn't EOF, we should
         * put that as the first char.
         */
        setp(writeBuffer, writeBuffer + kBufferSize);
        
        if(!traits_type::eq_int_type(traits_type::eof(), value))
            sputc(value);
        
        /* Success! */
        return traits_type::not_eof(value);
    }
    
    /* To synchronize the buffer and the socket, we just overflow with EOF
     * to force a write.
     */
    int NetStream::SocketStreambuf::sync() {
        return traits_type::eq_int_type(overflow(traits_type::eof()), traits_type::eof()) ? -1 : 0;
    }
    
    /* To receive data, we'll call recv and wait for things to show up. */
    NetStream::SocketStreambuf::int_type NetStream::SocketStreambuf::underflow() {
        /* If we are in a fail state, automatically abort. */
        if(!good)
            return traits_type::eof();
        
        if(gptr() == egptr()) {
            int numRead = (int)recv(sock, readBuffer, kBufferSize, 0);
            
            /* Check that we read good things.  0 or -1 indicates a closed connection. */
            if(numRead == -1 || numRead == 0) {
                good = false; // Fail all future reads.
                return traits_type::eof();
            }
            
            /* Okay, we read things.  Now set the pointers up. */
            setg(readBuffer, readBuffer, readBuffer + numRead);
        }
        return *gptr();
    }
    
    /* Put back the character, unless we're out of room. */
    NetStream::SocketStreambuf::int_type NetStream::SocketStreambuf::pbackfail(int_type ch) {
        /* If we don't have room, fail immediately. */
        if (eback() == egptr())
            return traits_type::eof();
        
        /* Back up a step. */
        gbump(-1);
        
        /* If the character isn't EOF, write it back. */
        if (traits_type::eq_int_type(ch, traits_type::eof()))
            *gptr() = ch;
        
        /* Return something other than EOF to signal success. */
        return traits_type::not_eof(ch);
    }
    
    /* NetStream constructors initialize the data members, tell the superclass
     * ctor that all is well, and tie itself to itself so that reads force a write.
     */
    
    /* Client constructor */
    NetStream::NetStream(const ukn_string& hostName, uint16 portNum):
    iostream(NULL), 
    connection(new SocketStreambuf()) {
        init(connection);
        
        /* We tie the stream to itself so that if we try to read,
         * we first flush all of the buffered data.  Otherwise,
         * an explicit flush would be required before any read.
         */
        tie(this);
        
        open(hostName, portNum);
    }
    
    /* Server constructor. */
    NetStream::NetStream(uint16 portNum):
    iostream(NULL), 
    connection(new SocketStreambuf(portNum)) {
        init(connection);
        tie(this);
        
        open(portNum);
    }
    
    /* Default constructor. */
    NetStream::NetStream(): 
    iostream(NULL), 
    connection(new SocketStreambuf) {
        init(connection);
        tie(this);
    }
    
    NetStream::~NetStream() {
        rdbuf(0); // Avoid weirdness if superclass tries to use the buffer.
        delete connection;
    }
    
    void NetStream::open(const ukn_string& filename, uint16 portNum) {
        if(!connection->open(filename, portNum))
            setstate(ios_base::failbit);
    }
    
    void NetStream::open(uint16 portNum) {
        if(!connection->open(portNum))
            setstate(ios_base::failbit);
    }
    
    void NetStream::close() {
        if(!connection->close())
            setstate(ios_base::failbit);
    }
    
    bool NetStream::canRead() const {
    	return true;
    }
    
	bool NetStream::canWrite() const {
		return true;
	}
	
    bool NetStream::canSeek() const {
    	return false;
    }
    
    bool NetStream::is_open() const {
        return connection->isOpen();
    }
    
    bool NetStream::seek(size_t pos) {
        return false;
    }
    
    void NetStream::flush() {
        std::iostream::flush();
    }
    
    size_t NetStream::read(uint8* buffer, size_t length) {
        return 0;
    }
   
    size_t NetStream::write(const uint8* buffer, size_t length) {
        ((iostream*)this)->write((const char*)buffer, length);
        return 0;
    }
    
    size_t NetStream::pos() const {
        return 0;
    }
    
    size_t NetStream::size() const {
        return 0;
    }
    
    bool NetStream::isValid() const {
        return is_open();
    }
} // namespace ukn
