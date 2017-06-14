#ifndef __GELF_LOAD_BALANCE_DEFINE_H__
#define __GELF_LOAD_BALANCE_DEFINE_H__

#define MAX_SIZE 16384
#define DEFAULT_CHUNKED_TIMEOUT 5

typedef boost::array<char, MAX_SIZE> Buffer;

typedef std::shared_ptr<Buffer> SharedBufferInner;

struct SharedBuffer {
    size_t size;
    SharedBufferInner buff;
};


typedef std::vector<SharedBuffer> BufferStack;



#endif
