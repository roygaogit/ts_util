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
#include <utility>
#include <iomanip>
//  g++ pes_bitrate_calc.cpp -lts_util -lboost_system -lboost_program_options -lboost_filesystem

using namespace std;
using ts_util::TransportStream;
using ts_util::TSPacket;
using ts_util::NULL_PACKET_PID;
using boost::lexical_cast;
using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;
#include <iostream>
#include <iomanip>

using namespace std;


int main (int argc, char** argv)
{

	options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("sourcets,sts", value<string>()->required(),
		 "ts file to analyze");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")){
		cout << desc << '\n';
		return 0;
	}
	po::notify(vm);

	TransportStream tsInput(vm["sourcets"].as<string>());

	map<int, int> pidToPacketAmount;

	for (int i = 0; i < tsInput.getSizeInPackets(); i++){
		tsInput.goToPacket(i);
		pidToPacketAmount[tsInput.getCurrentPacket().getPID()]++;
	}

	cout << "\nOf a total of " << tsInput.getSizeInPackets() << " packets(" 
		 << tsInput.getSizeInPackets() * 188 <<" bytes, " << tsInput.calculateBitrate() <<" bps), detail is: " << '\n';
	
	cout << setfill('-') << setw(1) << "+" << setw(15) << "-" 
	     << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" << endl;
	cout << setfill(' ') << setw(1) << "|" << setw(15) << left << "PID" 
	     << setw(1) << "|" << setw(15) << left << "Bytes" 
	     << setw(1) << "|"  << setw(15) << left << "Percentage" 
	     << setw(1) << "|" << setw(15) << left << "Mbps"
	     << setw(1) << "|" << endl;
	cout << setfill('-') << setw(1) << "+" << setw(15) << "-" 
	     << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" << endl;

	long long bitrate = tsInput.calculateBitrate();
	long long totalBytes = tsInput.getSizeInPackets() * 188;
	typedef std::map<int, int> map_type;
	BOOST_FOREACH(map_type::value_type &p, pidToPacketAmount){
		long long per = p.second * 1000LL / tsInput.getSizeInPackets();
		long long mbps = p.second * 188LL * bitrate / 100000LL / totalBytes;
		cout << setfill(' ') << setw(1) << "|" << setw(15) << left << p.first 
			 << setw(1) << "|" << setw(15) << left << p.second * 188 
			 << setw(1) << "|" << setw(13) << right << per / 10 << "." << per % 10 
			 << setw(1) << "|" << setw(13) << right << mbps / 10 << "." << mbps % 10 
			 << setw(1) << "|" << endl;
		
		cout << setfill('-') << setw(1) << "+" << setw(15) << "-" 
	     << setw(1) << "+" << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" 
	     << setw(15) << "-" << setw(1) << "+" << endl;
	}

	return 0;
}






