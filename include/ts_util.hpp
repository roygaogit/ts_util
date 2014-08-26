#pragma once

#include <cstdio>
#include <string>
namespace ts_util{

const int NULL_PACKET_PID = 8191;
const unsigned int TS_PACKET_SIZE = 188;
const char TS_SYNC_BYTE = 'G';

class TSPacket;
class TransportStream;

class TransportStream{
public:
	TransportStream(const std::string& pathToTsFile, bool = false);
	~TransportStream();
	int getSizeInPackets();
	TSPacket getCurrentPacket();
	void next();
	void previous();
	void writePacketInPosition(const TSPacket &aPacket);
	bool hasPacket();
	//** Index base is 0
	void goToPacket(int);
	long long calculateBitrate();
protected:
private:
	FILE *_ts;
	unsigned int _currentPacket;
	int _sizeOfFile;
	std::string _tsPath;
};
	
class TSPacket{
public:
	TSPacket();
	bool isValid();
	bool hasPCR();
	int getPID();
	long long getPCR();

	unsigned char contents[TS_PACKET_SIZE];

protected:
private:
};

}
