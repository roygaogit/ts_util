#include <boost/filesystem.hpp>
#include <ts_util.hpp>
#include <stdexcept>
#include <map>
#include <utility>
namespace ts_util{


TransportStream::TransportStream(const std::string& pathToTsFile, bool forceCreate)
	:_currentPacket(0), _sizeOfFile(0), _tsPath(pathToTsFile){

	if (forceCreate && boost::filesystem::exists( pathToTsFile )){
		throw std::invalid_argument("Trying to overwrite " + pathToTsFile);
	}
	const char *mode;
	if (forceCreate){
		mode = "w+b";
	} else {
		mode = "r+b";
	}

	if (!forceCreate && boost::filesystem::file_size(pathToTsFile) % TS_PACKET_SIZE != 0){
		throw std::invalid_argument("Considering file size, it's posible this is not a ts file.");
	}

	if (!forceCreate){
		_sizeOfFile = boost::filesystem::file_size(pathToTsFile);
	}
	_ts = fopen( pathToTsFile.c_str(), mode);
	if (_ts == 0){
		std::string err = std::string("File could not be opened. Tried ") + std::string(mode) + std::string( " mode, for file: ") + pathToTsFile;
		perror (err.c_str());
	}
}

TransportStream::~TransportStream(){
	fclose(_ts);
}

TSPacket TransportStream::getCurrentPacket(){
	TSPacket cur;
	if (!hasPacket()){
		throw std::out_of_range("["+ _tsPath +"]There are no more packets in this ts!");
	}
	if (fread(cur.contents, TS_PACKET_SIZE, 1, _ts) != 1){
		throw std::out_of_range("["+ _tsPath +"]Could not read an entire packet! Invalid ts?");
	}
	if (!cur.isValid()){
		throw std::domain_error("["+ _tsPath +"]TSPacket is not valid! Invalid ts or lost sync!");
	}
	goToPacket(_currentPacket);
	return cur;
}

int TransportStream::getSizeInPackets(){
	return _sizeOfFile / TS_PACKET_SIZE;
}

bool TransportStream::hasPacket(){
	return _currentPacket < getSizeInPackets(); // feof falla, por eso fue cambiado.
}

void TransportStream::next(){
	goToPacket(_currentPacket + 1);
}

void TransportStream::previous(){
	goToPacket(_currentPacket - 1);
}

void TransportStream::goToPacket(int p){
	_currentPacket = p;
	fseek(_ts, TS_PACKET_SIZE * _currentPacket, SEEK_SET);
}


void TransportStream::writePacketInPosition(const TSPacket &aPacket){
	fwrite(aPacket.contents, TS_PACKET_SIZE, 1, _ts);
	goToPacket(_currentPacket);
}

long long TransportStream::calculateBitrate(){
	int savedCurrentPacket = _currentPacket;

	goToPacket(0);
	bool keepSearching = true;
	std::map<int, std::pair<int, long long> > pidToPCR;
	int packetsDiff = -1;
	long long pcrDiff = -1;
	for (int i = 0; i < getSizeInPackets() && keepSearching; i++){
		TSPacket tsp = getCurrentPacket();
		next();
		if (tsp.hasPCR()){
			int pid = tsp.getPID();
			if (pidToPCR.count(pid)){
				packetsDiff = i - pidToPCR[pid].first;
				pcrDiff = tsp.getPCR() - pidToPCR[pid].second;
				keepSearching = false;
			} else {
				pidToPCR[pid] = std::make_pair(i, tsp.getPCR());
			}
		}
	}
	if (pcrDiff < 1){
		throw std::domain_error("PCR difference is not positive!");
	}
	long long bitrate = packetsDiff * TS_PACKET_SIZE * 8 * 27000000LL / pcrDiff;
	goToPacket(savedCurrentPacket);
	return bitrate;
}


/************************************************************
 * TS PACKET definitions
 ************************************************************/

TSPacket::TSPacket(){

}
bool TSPacket::isValid(){
	return this->contents[0] == TS_SYNC_BYTE;
}
int TSPacket::getPID(){
	return (( static_cast<int>(this->contents[1]) << 8) | this->contents[2]) & 0x1FFF;
}

bool TSPacket::hasPCR(){
	bool adaptation_field = (this->contents[3] & 0x20) != 0;
    if (adaptation_field && ((unsigned char)this->contents[4]) >= 7)
        return (this->contents[5] & 0x10) != 0;
    return false;
}

long long TSPacket::getPCR(){
	long long pcr_base(0);
    long long pcr_ext (0);	
    pcr_base =
        ((long long)this->contents[6] << 25) +
        (this->contents[ 7] << 17) +
        (this->contents[ 8] <<  9) +
        (this->contents[ 9] <<  1) +
        (this->contents[10] >>  7);
        pcr_ext = ((this->contents[10] & 0x01) << 8) + this->contents[11];

        return (pcr_base*300) + pcr_ext;
}





}