#ifndef __GELF_LOAD_BALANCE_PROTOCOL_HPP__
#define __GELF_LOAD_BALANCE_PROTOCOL_HPP__


#include <ctime>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>

#include "define.h"
#include "ChunkedMessage.hpp"

using namespace boost::property_tree; 

// chunked message
// 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16
// 1e 0f e2 05 bc 19 7a 90 b9 eb 00 04 78 9c ed 98 c7 ce a8 48 92 85 df 
//| F   |   id                  | I| T| message                           

typedef std::function<void (BufferStack)> DeStackCallback;

class ChunkedMessage;
typedef std::shared_ptr<ChunkedMessage> SharedChunkedMessage;

class Protocol {
public:

    Protocol(
        boost::asio::io_service& io,
        DeStackCallback callback,
        ptree& config);

    void enStack(SharedBuffer sb);

    void remove(std::string id);

    size_t __getIndex(SharedBufferInner sb);

    size_t __getTotal(SharedBufferInner sb);

    std::string __getId(SharedBufferInner sb);

    typedef std::unordered_map<std::string, SharedChunkedMessage> ChunkedMap;

private:
    boost::asio::io_service&    io_;
    ChunkedMap                  chunked_map_;
    DeStackCallback             callback_;
    size_t                      chunk_timeout_;
};


#endif
