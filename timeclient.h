#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>

using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using namespace std;


class TimeClient {
public:
	boost::asio::io_service io_service;
  	tcp::socket socket;
	deadline_timer deadline_;
  //	boost::asio::streambuf input_buffer_;
	boost::array<char, 8> buf;

    TimeClient() : deadline_(io_service), socket(io_service)  {};
    ~TimeClient() {};
    void write_handler(boost::shared_ptr<std::string> pstr, error_code ec,  size_t bytes_transferred);
	void read_handler(const boost::system::error_code& error);
	void start(char* server_ip);
	int read_ack(boost::posix_time::time_duration timeout);
};
#endif

