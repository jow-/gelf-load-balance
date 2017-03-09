#include "ChunkedMessage.hpp"


ChunkedMessage::ChunkedMessage(
    boost::asio::io_service& io,
    Protocol& protocol,
    std::string id,
    size_t total):
        io_(io),
        protocol_(protocol),
        id_(id),
        timer_(io, boost::posix_time::seconds(CHUNKED_TIMEOUT)),
        total_(total)
{
    buffer_stack_.resize(total);
    flag_.resize(total);
    for(int i=0; i<total; i++) {
        flag_[i] = 0;
    }
}

ChunkedMessage::~ChunkedMessage() {

}


void ChunkedMessage::enStack(size_t i, SharedBuffer sb) {

    buffer_stack_[i] = sb;
    flag_[i] = 1;
    last_ = time(NULL);
    timer_.cancel();
    timer_.async_wait(boost::bind(
         &ChunkedMessage::timeout,  shared_from_this(), _1));
}

bool ChunkedMessage::isFinish() {
    for (int i=0; i<total_; i++) {
        if (flag_[i] == 0)
            return false;
    }
    return true;
}

void ChunkedMessage::timeout(const boost::system::error_code& error_code) {
    if (!error_code) {
        protocol_.remove(id_);
    }
}