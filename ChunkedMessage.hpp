#ifndef __GELF_LOAD_BALANCE_CHUNKED_MESSAGE_HPP__
#define __GELF_LOAD_BALANCE_CHUNKED_MESSAGE_HPP__

#include <ctime>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "define.h"
#include "protocol.hpp"

class Protocol;

class ChunkedMessage :
    public std::enable_shared_from_this<ChunkedMessage> {
public:
    ChunkedMessage(
        boost::asio::io_service& io,
        Protocol& protocol,
        std::string id,
        size_t total,
        size_t chunk_timeout);

    ~ChunkedMessage();

    void enStack(size_t i, SharedBuffer sb);

    bool isFinish();

    BufferStack getAndRemoveMessage();

    std::string id() {
      return id_;
    }

    void timeout(const boost::system::error_code& error_code);
private:
    boost::asio::io_service&    io_;
    Protocol&                   protocol_;
    std::string                 id_;
    boost::asio::deadline_timer timer_;
    std::vector<int>            flag_;
    BufferStack                 buffer_stack_;
    size_t                      total_;
    time_t                      last_;
};



#endif
