#pragma once
#include "stdafx.h"
#include "ChannelRecord.h"


class ChannelGroup
{
	std::string _name;
	std::vector<std::unique_ptr<ChannelRecord>> _channels;

public:
	ChannelGroup(const std::string& name)
	{
		_name = name;
	}
	void AddChannel(std::unique_ptr<ChannelRecord>& channel);

	void SetChannelNumber(int& i);
	void WriteTD(std::ofstream& output);
	void WriteTable(std::ofstream& output);
};
