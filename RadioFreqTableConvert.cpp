// RadioFreqTableConvert1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "RadioFreqTableConvertVer.h"
#include "ChannelRecord.h"

int main(int argc, char* argv[])
{
    const int version[] = { RADIO_FREQ_TABLE_CONVERT_VER };
    std::cout << "Radio Frequency Table Converter from .CSV \"RT Systems file\" to TIDRADIO TD-H9\nVersion " 
        << version[0] << '.' << version[1] << '.' << version[2] << '.' << version[3] <<  std::endl << std::endl;
    if (argc < 3)
    {
        std::cout << "Please specify the source .CSV file and the destination .CSV file\n";
        return -1;
    }
    std::ifstream inputRtSystemsFile;
    std::ofstream outputTidRadioFile;

    inputRtSystemsFile.open(argv[1], std::ios_base::in);
    if (!inputRtSystemsFile.is_open())
    {
        char errorBuffer[256];
        strerror_s(errorBuffer, 256, errno);
        std::cerr << "Error: " << errorBuffer << " Could not open the file" << argv[1] << std::endl;
        return -1;
    }

    outputTidRadioFile.open(argv[2], std::ios_base::out);
    if (!inputRtSystemsFile.is_open())
    {
        char errorBuffer[256];
        strerror_s(errorBuffer, 256, errno);
        std::cerr << "Error: " << errorBuffer << " Could not create the file" << argv[2] << std::endl;
        return -1;
    }
    
    std::string line;
    std::vector<std::unique_ptr<ChannelRecord>> channels;
    int maxOriginalChannelNumber = 0;
    while (std::getline(inputRtSystemsFile, line)) 
    {
        if (line.length() == 0 || !std::isdigit(line[0]))
        {
            std::cout << "Ignored: " << line << std::endl;
            continue;
        }


        auto channel = std::make_unique<ChannelRecord>();
        if(!channel->ReadRT(line))
        {
            continue;
        }
        int originalChannelNumber = channel->OriginalChannelNumber();
        if (originalChannelNumber > maxOriginalChannelNumber)
        {
            maxOriginalChannelNumber = originalChannelNumber;
        }
        channels.push_back(std::move(channel));
    }
    const int TIDRADIO_H9_CHANNEL_MAX = 199;
    int channelNumberAdjuster = maxOriginalChannelNumber - TIDRADIO_H9_CHANNEL_MAX;
    if (channelNumberAdjuster > 0)
    {
        int i = 0;
        for (const auto& channel : channels) 
        {
            channel->SetChannelNumber(++i);
        }
    }

    outputTidRadioFile << "Channel No,RX Freq [MHz],TX Freq [MHz],RX CTCSS/DCS,TX CTCSS/DCS,Power,Bandwidth,Scrambler,PTT ID,Freq Hop,Busy Lock,Scan,Rx Model,Name" << std::endl;
    for (const auto& channel : channels)
    {
        line = channel->WriteTD();
        outputTidRadioFile << line << std::endl;
        std::cout << line << std::endl;
    }
    std::cout << "Wrote converted data to " << argv[2] << std::endl;
}

