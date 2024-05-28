
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
    int readDataInt(const std::string& mapKey) const;
    std::array<int,4> readDataArray(const std::string& mapKey) const;
private:
    std::multimap <std::string, int> valueEC;
    std::multimap < std::string, std::array<int,4>> vvalue;
    std::vector<int> tempVector;

};

class CRC
{
public:
    CRC();
    void Crc16(const std::vector<unsigned char>& crcSum);
    void retCrc(std::vector<unsigned char>& crcSum) const;
    void testCrc() const;
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
	Handling() : paramRead{ 256 } {}
	void switchRule(std::vector<uint8_t>& procVector);
	bool checkAdr(const std::vector<uint8_t>& procVector);
	void writeIntToVector(int valueDB, int bytesLen, std::vector<uint8_t>& lastVector);
	void writeIntToVectorCol(int valueDB, int bytesLen, std::vector<uint8_t>& lastVector);
	void arrayToIntVector(const std::string &mapKey,int size);
    void getValuesFromArray(int arg1,int arg2,std::vector<uint8_t>&procVector);
    void getParamsEC(const std::vector<uint8_t>& procVector);
    void getCrc();
    void getInstantValues(const std::vector<uint8_t>& procVector);
private:
	int paramRead; //ruleswitcher helper
	std::vector<int>tempVector;

};

#endif // !HANDLER_H

