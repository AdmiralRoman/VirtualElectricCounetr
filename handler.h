
#ifndef HANDLER_H
#define HANDLER_H



#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <map>
#include "json.h"



class DataBase {
    friend class Handling;
public:
    DataBase();
    int readData(std::string mapKey) const;
    std::array<int, 4> inputData(std::string mapKey);
private:
    std::multimap <std::string, int> valueEC;
    std::multimap < std::string, std::array<int,4>> vvalue;
    std::vector<int> tempVector;

};

class CRC
{
public:
    CRC();
    void Crc16(std::vector<unsigned char>& crcSum);
    void retCrc(std::vector<unsigned char>& crcSum);
    void testCrc();
private:

    unsigned char auchCRCHi[256];
    unsigned char auchCRCLo[256];
    bool checkCrc;
    unsigned char uchCRCHi; 
    unsigned char uchCRCLo;
};



class Handling 
{

public:
	Handling() : paramRead{ -1 } {}
	void switchRule(std::vector<uint8_t>& procVector);
	bool checkAdr(std::vector<uint8_t>& procVector);
	void writeIntToVector(int valueDB, int bytesLen, std::vector<uint8_t>& lastVector);
	void writeIntToVectorCol(int valueDB, int bytesLen, std::vector<uint8_t>& lastVector);
	void arrayToIntVector(std::string mapKey,int size);
private:
	int paramRead; //ruleswitcher helper
	std::vector<int>tempVector;

};

#endif // !HANDLER_H

