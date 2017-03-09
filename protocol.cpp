#include "protocol.hpp"


Protocol::Protocol(
  boost::asio::io_service& io, 
  DeStackCallback callback): 
    io_(io),
    callback_(callback) 
{

}

void Protocol::enStack(SharedBuffer shared_buffer) {
  std::cout << "Protocol::enStack" << std::endl;
  SharedBuffer_ sb = shared_buffer.buff;
  if (sb && sb->size() > 11) {
    std::string id = __getId(sb);
    if (id == "") {
      return ;
    }

    std::cout << "Protocol::enStack id " << id << std::endl;
    ChunkedMap::const_iterator it = chunked_map_.find(id);
    if (it != chunked_map_.end()) {
      std::cout << "Protocol::enStack find cm " << std::endl;
      SharedChunkedMessage cm = (*it).second;
      size_t index = __getIndex(sb);
      cm->enStack(index, shared_buffer);
      if (cm->isFinish()) {
        callback_(cm->getMessage());
        chunked_map_.erase(it);
      }
    } else {
      std::cout << "Protocol::enStack not find cm " << std::endl;
      size_t index = __getIndex(sb);
      size_t total = __getTotal(sb);
      std::cout << "Protocol::enStack index " << index << std::endl;
      std::cout << "Protocol::enStack total " << total << std::endl;
      SharedChunkedMessage cm(new ChunkedMessage(io_, *this, id, total));

      std::cout << "Protocol::enStack crete cm"  << std::endl;
      cm->enStack(index, shared_buffer);
      std::cout << "Protocol::enStack cm->enStack over"  << std::endl;
      if (cm->isFinish()) {
        callback_(cm->getMessage());
      } else {
        chunked_map_.insert(std::make_pair(id, cm));
      }
      std::cout << "Protocol::enStack finish "  << std::endl;
    }
  }
}

void Protocol::remove(std::string id) {
  chunked_map_.erase(id);
}

size_t Protocol::__getIndex(SharedBuffer_ sb) {
  return (*sb)[10];
}

size_t Protocol::__getTotal(SharedBuffer_ sb) {
  return (*sb)[11];
}

std::string Protocol::__getId(SharedBuffer_ sb) {
  std::string ret;
  for (int i=2; i<9; i++) {
    char tmp[5];
    sprintf(tmp, "%02X", (unsigned char)((*sb)[i]));
    ret.append(tmp);
  }
  return ret;
}