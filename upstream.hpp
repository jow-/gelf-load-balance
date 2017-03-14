#ifndef __GELF_LOAD_BALANCE_UPSTREAM_HPP__
#define __GELF_LOAD_BALANCE_UPSTREAM_HPP__

#include <iostream>
#include <vector>
#include <mutex>
#include <boost/property_tree/ptree.hpp>
#include <boost/asio.hpp>
#include <boost/foreach.hpp>


using namespace boost::asio::ip;
using namespace boost::property_tree; 

class Upstream {
public:
    Upstream(ptree& config): weight_total_(0), seed_(1) {
        ptree upstreams = config.get_child("upstream");

        BOOST_FOREACH(ptree::value_type &vt, upstreams) {
            std::string host = vt.second.get_child("host").get_value<std::string>();
            int port = vt.second.get_child("port").get_value<int>();
            int weight = vt.second.get_child("weight").get_value<int>();

            udp::endpoint endpoint(address_v4::from_string(host), port);
            upstreams_.push_back(endpoint);
            weight_total_ += weight;
            boundry_.push_back(weight_total_);
        }
    }

    udp::endpoint& get() {
        std::lock_guard<std::mutex> guard(mutex_);
        int rnd = random(weight_total_);

        for(int i=0; i<boundry_.size(); i++) {
            if (rnd < boundry_[i]) {
                return upstreams_[i];
            }
        }
        assert(false);
    }

    int random(int mod) {
        seed_ = (seed_ * 123 + 59)%65535;
        return seed_%mod;
    }

private: 
    mutable std::mutex mutex_;
    std::vector<int> boundry_;
    std::vector<udp::endpoint> upstreams_;
    int weight_total_;
    int seed_;
};



#endif

