#pragma once

#include "ts_util.hpp"
#include <string>
#include <boost/asio.hpp>


namespace ts_util{

using std::string;

int const PACKETS_PER_DATAGRAM = 7;

class TSStreamerOverRtp{

public:
	TSStreamerOverRtp(const string &, const unsigned short, TransportStream &);
	~TSStreamerOverRtp();
	void start();

protected:

private:
	string _ipAddress;
	unsigned short _port;
	TransportStream &_ts;
    boost::asio::io_service _ioService;
	boost::asio::ip::udp::socket _socket;
	boost::asio::ip::udp::endpoint _endpoint;
};

}