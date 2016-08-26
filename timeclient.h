#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
using namespace std;


class TimeClient {
public:
	boost::array<char, 8> buf;

    TimeClient() {};
    ~TimeClient() {};
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec,  size_t bytes_transferred);
	void read_handler(const boost::system::error_code& error);
	void start(boost::asio::io_service &io_service, char* server_ip);
	
};
#endif

