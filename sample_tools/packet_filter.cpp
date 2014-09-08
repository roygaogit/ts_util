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

//  g++ packet_filter.cpp -lts_util -lboost_system -lboost_program_options -lboost_filesystem


using ts_util::TransportStream;
using ts_util::TSPacket;
using std::cout;
using std::endl;
using std::set;
using std::string;
using std::vector;
using boost::lexical_cast;
using boost::program_options::options_description;
using boost::program_options::value;

namespace po = boost::program_options;

int64_t bytes_read = 0;



void filterPackets(TransportStream &tsInput, TransportStream &tsOutput, set<int>& pids, const string &mode){

	bool exclude = mode == "ex";

	while ( tsInput.hasPacket() ){
		TSPacket p = tsInput.getCurrentPacket();
		tsInput.next();
        int pid = p.getPID();
        if (exclude && !pids.count(pid) || !exclude && pids.count(pid)){
        	tsOutput.writePacketInPosition(p);
        	tsOutput.next();
        }
	}
}



int main (int argc, char** argv)
{

	options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("mode,m", value<string>()->required(), "can be either in or ex. Short for include or exclude")
		("input,i", value<string>()->required(), "path of the input file")
		("output,o", value<string>()->required(), "path of the output file")
		("pids,p", value< vector<int> >()->multitoken()->required(), "pids we'd like to filter/let pass");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help") ||
		(vm.count("mode") && vm["mode"].as<string>() != "in" && vm["mode"].as<string>() != "ex")
		){
		cout << desc << '\n';
		return 0;
	}

	po::notify(vm);

	set<int> pids;

	BOOST_FOREACH(int i, vm["pids"].as< vector<int> >()){
		pids.insert(i);
	}

	TransportStream tsInput(vm["input"].as<string>());
	TransportStream tsOutput(vm["output"].as<string>(), true); // EL true es para forzar la creacion del
														     // archivo.

	filterPackets(tsInput, tsOutput, pids, vm["mode"].as<string>());
	
	return 0;
}

