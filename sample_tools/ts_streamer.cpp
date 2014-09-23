#include <ts_util/ts_rtp_streamer.hpp>
#include <ts_util/ts_util.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <cassert>
#include <cstdio>
#include <set>
#include <string>
#include <vector>

//  Compilado con:
// g++ streamer.cpp -lts_util -lts_rtp_streamer -lboost_system -lboost_program_options -lboost_filesystem


using ts_util::TransportStream;
using ts_util::TSPacket;
using ts_util::TSStreamerOverRtp;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;
using boost::lexical_cast;
using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

int main (int argc, char** argv)
{

	options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("ts,ts", value<string>()->required(), "Path to the ts which we want to stream")
		("ip,ip", value<string>()->default_value("239.1.1.1"), "Multicast group ip address.")
		("port,p", value<int>()->default_value(8080), "Multicast group port.");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")){
		cout << desc << '\n';
		return 0;
	}

	po::notify(vm);
	TransportStream ts(vm["ts"].as<string>());
	cout << "Streaming at " << ts.calculateBitrate() << " bps.\n";
	TSStreamerOverRtp s(vm["ip"].as<string>(), vm["port"].as<int>(), ts);
	s.start();
}
