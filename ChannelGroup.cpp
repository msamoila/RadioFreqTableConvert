#include "stdafx.h"
#include "ChannelGroup.h"

void ChannelGroup::AddChannel(std::unique_ptr<ChannelRecord>& channel)
{
	_channels.push_back(std::move(channel));	
}

void ChannelGroup::SetChannelNumber(int& i)
{
	for (const auto& channel : _channels)
	{
		channel->SetChannelNumber(++i);
	}
}

void ChannelGroup::WriteTD(std::ofstream& output)
{
    for (const auto& channel : _channels)
    {
        std::string line = channel->WriteTD();
        output << line << std::endl;
    }
}

void ChannelGroup::WriteTable(std::ofstream& output)
{
    output << _name << std::endl;
    for (const auto& channel : _channels)
    {
        std::string line = channel->WriteTable();
        output << line << std::endl;
    }
}
