#pragma once
#include "stdafx.h"
#include "Enums.h"

std::string CtssToString(double ctss)
{
	if (ctss == 0.)
	{
		return "OFF";
	}
	else
	{
		return std::format("{:g}", ctss);
	}
}

std::string TxPowerToString(TxPower txPower)
{
	switch (txPower)
	{
	case High:
		return "High";
	case Middle:
		return "Middle";
	case Low:
		return "Low";
	default:
		return "";
	}
}

std::string BandwidthForFrequency(double txFrequency)
{
	const int BAND_70CM_LOWER_LIMIT_MHZ = 420;
	return txFrequency > BAND_70CM_LOWER_LIMIT_MHZ ? "Wide" : "Narrow";
}