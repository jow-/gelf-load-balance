#include "protocol.hpp"


Protocol::Protocol(
  boost::asio::io_service& io, 
  DeStackCallback callback): 
    io_(io),
    callback_(callback) 
{

}

void Protocol::enStack(SharedBuffer shared_buffer) {
    // std::cout << "Protocol::enStack" << std::endl;
    SharedBufferInner sb = shared_buffer.buff;
    if (sb && sb->size() > 11) {
        std::string id = __getId(sb);
        if (id == "") {
            return ;
        }
        ChunkedMap::const_iterator it = chunked_map_.find(id);
        if (it != chunked_map_.end()) {
            SharedChunkedMessage cm = (*it).second;
            size_t index = __getIndex(sb);
            cm->enStack(index, shared_buffer);
            if (cm->isFinish()) {
                callback_(cm->getMessage());
                remove(id);
            }
        } else {
            size_t index = __getIndex(sb);
            size_t total = __getTotal(sb);
            SharedChunkedMessage cm(new ChunkedMessage(io_, *this, id, total));

            cm->enStack(index, shared_buffer);
            if (cm->isFinish()) {
                callback_(cm->getMessage());
                remove(id);
            } else {
                chunked_map_.insert(std::make_pair(id, cm));
            }
        }
    }
}

void Protocol::remove(std::string id) {
    chunked_map_.erase(id);
}

size_t Protocol::__getIndex(SharedBufferInner sb) {
    return (*sb)[10];
}

size_t Protocol::__getTotal(SharedBufferInner sb) {
    return (*sb)[11];
}

std::string Protocol::__getId(SharedBufferInner sb) {
    std::string ret;
    for (int i=2; i<9; i++) {
        char tmp[5];
        sprintf(tmp, "%02X", (unsigned char)((*sb)[i]));
        ret.append(tmp);
    }
    return ret;
}