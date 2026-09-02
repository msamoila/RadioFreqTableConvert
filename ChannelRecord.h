#pragma once
#include "stdafx.h"

enum TxPower
{
	Low,
	Middle,
	High
};


class ChannelRecord {
	int				_originalChannelNumber;
	int				_channelNumber;
	std::string		_name;
	double			_rxFrequency;
	double			_txFrequency;
	double			_cTCSS;
	double			_rxCTCSS;
	TxPower			_txPower;

public:
	ChannelRecord()
	{
		_channelNumber = _originalChannelNumber = 0;
		_rxFrequency = _txFrequency = 0;
		_cTCSS = _rxCTCSS = 0.;
		_txPower = High;
	}

	int OriginalChannelNumber() const 
	{
		return _originalChannelNumber;
	}
	void SetChannelNumber(int channelNumber)
	{
		_channelNumber = channelNumber;
	}

	bool ReadRT(std::string line);
	std::string WriteTD();
};
