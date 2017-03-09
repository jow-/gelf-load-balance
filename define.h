#ifndef __GELF_LOAD_BALANCE_DEFINE_H__
#define __GELF_LOAD_BALANCE_DEFINE_H__

#define MAX_SIZE 1500
#define CHUNKED_TIMEOUT 3000

typedef boost::array<char, MAX_SIZE> Buffer;

typedef std::shared_ptr<Buffer> SharedBufferInner;

struct SharedBuffer {
  size_t size;
  SharedBuffer_ buff;
};


typedef std::vector<SharedBuffer> BufferStack;



#endif
