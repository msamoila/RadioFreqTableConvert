// RadioFreqTableConvert1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "RadioFreqTableConvertVer.h"
#include "ChannelRecord.h"
#include "ChannelGroup.h"

int main(int argc, char* argv[])
{
    const int version[] = { RADIO_FREQ_TABLE_CONVERT_VER };
    std::cout << "Radio Frequency Table Converter from .CSV \"RT Systems file\" to TIDRADIO TD-H9\nVersion " 
        << version[0] << '.' << version[1] << '.' << version[2] << '.' << version[3] <<  std::endl << std::endl;
    if (argc < 3)
    {
        std::cout << "Please specify the source .CSV file, the destination .CSV file and optionally the .CSV file with channel listing\n";
        return -1;
    }
    std::ifstream inputRtSystemsFile;
    std::ofstream outputTidRadioFile;
    std::ofstream outputReferenceRadioFile;

    inputRtSystemsFile.open(argv[1], std::ios_base::in);
    if (!inputRtSystemsFile.is_open())
    {
        char errorBuffer[256];
        strerror_s(errorBuffer, 256, errno);
        std::cerr << "Error: " << errorBuffer << " Could not open the file" << argv[1] << std::endl;
        return -1;
    }

    outputTidRadioFile.open(argv[2], std::ios_base::out);
    if (!outputTidRadioFile.is_open())
    {
        char errorBuffer[256];
        strerror_s(errorBuffer, 256, errno);
        std::cerr << "Error: " << errorBuffer << " Could not create the file" << argv[2] << std::endl;
        return -1;
    }

    if (argc > 3)
    {
        outputReferenceRadioFile.open(argv[3], std::ios_base::out);
        if (!outputReferenceRadioFile.is_open())
        {
            char errorBuffer[256];
            strerror_s(errorBuffer, 256, errno);
            std::cerr << "Error: " << errorBuffer << " Could not create the file" << argv[3] << std::endl;
            return -1;
        }
    }
    
    std::string line, previousIgnoredLine;
    std::vector<std::unique_ptr<ChannelGroup>> channelGroups;
    std::unique_ptr<ChannelGroup> currentChannelGroup;
    int maxOriginalChannelNumber = 0;
    while (std::getline(inputRtSystemsFile, line)) 
    {
        if (line.length() == 0 || !std::isdigit(line[0]))
        {
            std::cout << "Ignored: " << line << std::endl;
            previousIgnoredLine = line;
            continue;
        }


        auto channel = std::make_unique<ChannelRecord>();
        if(!channel->ReadRT(line))
        {
            continue;
        }
        if (!previousIgnoredLine.empty())
        {
            if (currentChannelGroup)
            {
                channelGroups.push_back(std::move(currentChannelGroup));
            }
            currentChannelGroup = std::make_unique<ChannelGroup>(previousIgnoredLine);
            previousIgnoredLine.clear();
        }
        int originalChannelNumber = channel->OriginalChannelNumber();
        if (originalChannelNumber > maxOriginalChannelNumber)
        {
            maxOriginalChannelNumber = originalChannelNumber;
        }
        currentChannelGroup->AddChannel(channel);
    }
    if (currentChannelGroup)
    {
        channelGroups.push_back(std::move(currentChannelGroup));
    }
    const int TIDRADIO_H9_CHANNEL_MAX = 199;
    int channelNumberAdjuster = maxOriginalChannelNumber - TIDRADIO_H9_CHANNEL_MAX;
    if (channelNumberAdjuster > 0)
    {
        int i = 0;
        for (const auto& channelGroup : channelGroups)
        {
            channelGroup->SetChannelNumber(i);
        }
    }

    outputTidRadioFile << "Channel No,RX Freq [MHz],TX Freq [MHz],RX CTCSS/DCS,TX CTCSS/DCS,Power,Bandwidth,Scrambler,PTT ID,Freq Hop,Busy Lock,Scan,Rx Model,Name" << std::endl;
    for (const auto& channelGroup : channelGroups)
    {
        channelGroup->WriteTD(outputTidRadioFile);
    }
    std::cout << std::endl << "Wrote converted data to " << argv[2] << std::endl;

    if (outputReferenceRadioFile.is_open())
    {
        outputReferenceRadioFile << "Channel,Original ch,RX Freq [MHz],TX Freq [MHz],Name,RX CTCSS/DCS,TX CTCSS/DCS,Power,Bandwidth,Comment" << std::endl;
        for (const auto& channelGroup : channelGroups)
        {
            channelGroup->WriteTable(outputReferenceRadioFile);
        }
        std::cout << std::endl << "Wrote channel listing to " << argv[3] << std::endl;
    }
}

