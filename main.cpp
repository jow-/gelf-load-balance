#include <iostream>
#include <functional>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <unordered_map>

#include "define.h"
#include "protocol.hpp"
#include "upstream.hpp"

using namespace boost::asio::ip;
using namespace boost::property_tree; 

namespace UdpAsyService  
{  
    class udp_server  
    {  
    public:  
        udp_server(
            boost::asio::io_service& io,
            udp::endpoint endpoint,
            ptree& config)  
            :socket_(io, endpoint),
             protocol_(io, boost::bind(&udp_server::hand_chunk, this, _1), config),
             upstream_(config)
        {
            start_recive();  
        }
  
        void start_recive()  
        {  
            socket_.async_receive_from(boost::asio::buffer(rec_buf_), remot_endpoint_,  
                boost::bind(&udp_server::hand_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));  
        }

        void hand_chunk(BufferStack buffer_stack) 
        {
            udp::endpoint &up_endpoint = upstream_.get();
            std::for_each(buffer_stack.begin(), buffer_stack.end(),[&](SharedBuffer& sb){
                socket_.async_send_to(boost::asio::buffer(*(sb.buff), sb.size), up_endpoint,
                      boost::bind(&udp_server::hand_send, this, sb.buff, sb.size, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            });

        }
  
    private:  
        void hand_receive(const boost::system::error_code& error, std::size_t size)  
        {  
            if (!error || error != boost::asio::error::message_size)  
            {
                SharedBufferInner copy = SharedBufferInner(new Buffer());
                boost::swap(rec_buf_, *copy);
                if ((*copy)[0] == 0x78 || (*copy)[0] == 0x1f) {
                    udp::endpoint &up_endpoint = upstream_.get();
                    socket_.async_send_to(boost::asio::buffer(*copy, size), up_endpoint,
                        boost::bind(&udp_server::hand_send, this, copy, size, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));  
                } else if ((*copy)[0] == 0x1e) {
                    SharedBuffer sb;
                    sb.size = size;
                    sb.buff = copy;
                    protocol_.enStack(sb);
                }
                start_recive();//next client;  
            }  
        }

        void hand_send(SharedBufferInner buff, size_t buff_len, const boost::system::system_error& error, std::size_t size)
        {  
//            std::cout << "send byte: " << buff_len << std::endl;
        }  
  
    private:  
        udp::socket socket_;  
        udp::endpoint remot_endpoint_;
        Buffer rec_buf_;
        Protocol protocol_;
        Upstream upstream_;
    };  
  
    void udp_asy_server(
            udp::endpoint endpoint,
            ptree& config)  
    {  
        try
        {
            boost::asio::io_service io;  
            udp_server server(io, endpoint, config);  
  
            io.run();  
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }  
}

int main(int argc, char* argv[])
{
    std::string filename = "/etc/lss.conf.d/gelf-load-balance.json";
    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {
            filename = argv[i+1];
            break;
        }
    }
    std::cout << filename << std::endl;
    ptree config;
    read_json(filename, config);

    std::vector<udp::endpoint> endpoints;

    std::string host = config.get_child("host").get_value<std::string>();
    int port = config.get_child("port").get_value<int>();

    udp::endpoint endpoint(address_v4::from_string(host), port);

    UdpAsyService::udp_asy_server(endpoint, config);
    return 0;
}