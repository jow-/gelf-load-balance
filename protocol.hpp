#ifndef __GELF_LOAD_BALANCE_PROTOCOL_HPP__
#define __GELF_LOAD_BALANCE_PROTOCOL_HPP__

#include <ctime>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <boost/array.hpp>

const size_t MAX_SIZE = 1500;

typedef boost::array<char, MAX_SIZE> Buffer;
typedef std::shared_ptr<Buffer> SharedBuffer;
typedef std::vector<SharedBuffer> BufferStack;

class ChunkedMessage {
public:
  ChunkedMessage(size_t total) : _total(total) {
    _buffer_stack.resize(total);
    for(int i=0; i<total; i++) {
      _flag[i] = 0;
    }
  }

  void enStack(size_t i, SharedBuffer sb) {
    _buffer_stack[i] = sb;
    _flag[i] = 1;
    _last = time(NULL);
  }

  bool isFinish() {
    for (int i=0; i<_total; i++) {
      if (_flag[i] == 0)
        return false;
    }
    return true;
  }

  BufferStack getMessage() {
    return _buffer_stack;
  }

private:
  std::vector<int> _flag;
  BufferStack _buffer_stack;
  size_t      _total;
  time_t      _last;
};

typedef std::shared_ptr<ChunkedMessage> SharedChunkedMessage;

// chunked message
// 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16
// 1e 0f e2 05 bc 19 7a 90 b9 eb 00 04 78 9c ed 98 c7 ce a8 48 92 85 df 
//| F   |   id                  | I| T| message                           

typedef std::function<void (BufferStack)> DeStackCallback;

class Protocol {
public:

  Protocol(DeStackCallback callback): _callback(callback) {}

  void enStack(SharedBuffer sb) {
    if (sb && sb->size() > 11) {
      std::string id = __getId(sb);
      if (id == "") {
        return ;
      }

      ChunkedMap::const_iterator it = _chunked_map.find(id);
      if (it != _chunked_map.end()) {
        SharedChunkedMessage cm = (*it).second;
        size_t index = __getIndex(sb);
        cm->enStack(index, sb);
        if (cm->isFinish()) {
          _callback(cm->getMessage());
          _chunked_map.erase(it);
        }
      } else {
        size_t index = __getIndex(sb);
        size_t total = __getTotal(sb);
        SharedChunkedMessage cm = SharedChunkedMessage(new ChunkedMessage(total));
        cm->enStack(index, sb);
        if (cm->isFinish()) {
          _callback(cm->getMessage());
        } else {
          _chunked_map[id] = cm;
        }
      }
    }
  }

  size_t __getIndex(SharedBuffer sb) {
    return (*sb)[10];
  }

  size_t __getTotal(SharedBuffer sb) {
    return (*sb)[11];
  }

  std::string __getId(SharedBuffer sb) {
    std::string ret;
    for (int i=2; i<9; i++) {
      char tmp[5];
      sprintf(tmp, "%02X", (unsigned char)((*sb)[i]));
      ret.append(tmp);
    }
    return ret;
  }

  typedef std::unordered_map<std::string, SharedChunkedMessage> ChunkedMap;

private:
  ChunkedMap        _chunked_map;
  DeStackCallback   _callback;
};




#endif
