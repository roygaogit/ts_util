#include <ts_rtp_streamer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 
#include <iostream>
#include <boost/foreach.hpp>
#include <stdexcept>
namespace ts_util{
using std::cout;
using std::vector;
TSStreamerOverRtp::TSStreamerOverRtp(const string &ipAddress, const unsigned short port, TransportStream &ts)
	: _ipAddress(ipAddress), _port(port), _ts(ts), 
	  _socket(_ioService),
	  _endpoint(boost::asio::ip::address::from_string(ipAddress), _port ){

}
TSStreamerOverRtp::~TSStreamerOverRtp(){

}
void TSStreamerOverRtp::start(){
	try{
		_socket.connect(_endpoint);
	} catch (boost::system::system_error &ex){
		cout << "UNABLE TO CONNECT\n";
		throw ex;
	}
	cout << "Connection established successfully!\n";
	long long bitrate = _ts.calculateBitrate();
	vector<unsigned char> buff;
	for (;;_ts.goToPacket(0))
		while(_ts.hasPacket()){
			namespace pt = boost::posix_time;
			pt::ptime now1 = pt::microsec_clock::universal_time();
			buff.resize(0);
			for (int i = 0; _ts.hasPacket() && i < PACKETS_PER_DATAGRAM; i++){
				TSPacket cur = _ts.getCurrentPacket();
				_ts.next();
				for (int i = 0; i < TS_PACKET_SIZE; i++ ){
					buff.push_back(cur.contents[i]);
				}
			}
			///....do something here...
			_socket.send(boost::asio::buffer(&(buff.front()), buff.size()));
			
			
			//cout << "Buff size " << buff.size() << '\n';
			pt::time_duration shouldWait =  pt::microseconds(buff.size() * 8LL * 1000000LL / bitrate);
			//boost::this_thread::sleep( shouldWait - dur /*pt::microseconds(140LL)*/ );
			pt::ptime now2;
			do{
				 now2 = pt::microsec_clock::universal_time();
			}while((now2 - now1) < shouldWait);
			//cout << "waiting " << (shouldWait - dur).total_microseconds() << '\n';
			//boost::posix_time::microseconds(20LL)
			// Probar luego con busy waiting.
		}
}

}