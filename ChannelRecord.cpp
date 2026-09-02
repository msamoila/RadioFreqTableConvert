#include "stdafx.h"
#include "StringUtils.h"
#include "ChannelRecord.h"


bool ChannelRecord::ReadRT(std::string line)
{
	std::vector<std::string> columns = split_by_char(line, ',');

	const int COL_COUNT = 16;
	if (columns.size() < COL_COUNT)
	{
		return false;
	}
	//0  1               2                  3       4                  5                 6                   7      8         9                                10  11         12         13         14    15
	// ,               , RT Systems Header, Name,   Receive Frequency, Offset Frequency, Transmit Frequency, CTCSS, Rx CTCSS, Comment,                           , Tx Power,  Show Name, Tone Mode, Skip, Bank
	//1, 145.450 VE7RVF, 01 RVF,            VE7RVF, 145.45,            -0.6,             144.85,             100,           , VE7RVF (location: North),          , Low,       1,         Tone,      Off,  A
	//6, 147.420 S3,     06 7.420,          147.42, 147.42,          ,                   147.42,             123,   123,      Simplex 3: Sec: Vancouver/Surrey, 4, High,      0,         T Sql,     Off,  A
	_originalChannelNumber = _channelNumber = std::stoi(columns[0]);
	_name = columns[3];
	_rxFrequency = std::stod(columns[4]);
	_txFrequency = std::stod(columns[6]);
	if (columns[7].length())
	{
		_cTCSS = std::stod(columns[7]);
	}
	if (columns[8].length())
	{
		_rxCTCSS = std::stod(columns[8]);
	}
	if (columns[11] == "Low")
	{
		_txPower = Low;
	}
	else if (columns[10] == "Middle")
	{
		_txPower = Middle;
	}
	else
	{
		_txPower = High;
	}
	return true;
}

std::string ChannelRecord::WriteTD()
{
	//Channel No, RX Freq [MHz], TX Freq [MHz], RX CTCSS/DCS, TX CTCSS/DCS, Power, Bandwidth, Scrambler, PTT ID, Freq Hop, Busy Lock, Scan, Rx Model, Name
	std::ostringstream oss;
	oss << _channelNumber << ','  << _rxFrequency << ',' << _txFrequency << ',';
	if (_rxCTCSS == 0.)
	{
		oss << "OFF,";
	}
	else
	{
		oss << _rxCTCSS << ',';
	}

	if (_cTCSS == 0.)
	{
		oss << "OFF,";
	}
	else
	{
		oss << _cTCSS << ',';
	}
	switch (_txPower)
	{
	case High:
		oss << "High,";
		break;
	case Middle:
		oss << "Middle,";
		break;
	case Low:
		oss << "Low,";
		break;
	}

	const int BAND_70CM_LOWER_LIMIT_MHZ = 420;
	const std::string bandWidth = _txFrequency > BAND_70CM_LOWER_LIMIT_MHZ ? "Wide" : "Narrow";

	oss << bandWidth << ",0,Off,OFF,OFF,ON,FM," << _name;
	return oss.str();
}