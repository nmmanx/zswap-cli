/**
 * SPDX-FileCopyrightText: 2020-2021 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <regex>

#include <fmt/format.h>
#include <semver.hpp>

#include "ksysversion/ksysversion.hpp"
#include "zswapobject/zswapobject.hpp"
#include "zswapworker/zswapworker.hpp"

bool ZSwapObject::CheckPercent(const std::string& Value)
{
    if (Value.empty()) return true;
    int ValueInt = std::stoi(Value);
    return (ValueInt < 0) || (ValueInt > 100);
}

bool ZSwapObject::CheckEnabled(const std::string& Value)
{
    return !std::regex_match(Value, std::regex("^[YN]$"));
}

bool ZSwapObject::CheckKernelVersion(const std::string& RequiredKernelVersion)
{
    return semver::from_string(std::make_unique<KSysVersion>() -> GetKernelVersion()) < semver::from_string(RequiredKernelVersion);
}

void ZSwapObject::WriteLogEntry(const std::string& Name, const std::string& NewValue, const std::string& OldValue)
{
    std::cout << fmt::format("Writing a new value \"{1}\" to the \"{0}\" variable. The old value was: \"{2}\".", Name, NewValue, OldValue) << std::endl;
}

std::string& ZSwapObject::GetZSwapEnabled()
{
    return ZSwapObject::ZSwapEnabled;
}

void ZSwapObject::SetZSwapEnabled(const std::string& Value)
{
    if (CheckEnabled(Value)) throw std::invalid_argument("The value of ZSwapEnabled is incorrect (only Y or N are supported).");
    WriteLogEntry(ZSwapObject::ZSwapEnabledName, Value, ZSwapObject::ZSwapEnabled);
    ZSwapObject::ZSwapEnabled = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapEnabledName, Value);
}

std::string& ZSwapObject::GetZSwapSameFilledPages()
{
    return ZSwapObject::ZSwapSameFilledPages;
}

void ZSwapObject::SetZSwapSameFilledPages(const std::string& Value)
{
    if (CheckEnabled(Value)) throw std::invalid_argument("The value of ZSwapSameFilledPages is incorrect (only Y or N are supported).");
    WriteLogEntry(ZSwapObject::ZSwapSameFilledPagesName, Value, ZSwapObject::ZSwapSameFilledPages);
    ZSwapObject::ZSwapSameFilledPages = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapSameFilledPagesName, Value);
}

std::string& ZSwapObject::GetZSwapMaxPoolPercent()
{
    return ZSwapObject::ZSwapMaxPoolPercent;
}

void ZSwapObject::SetZSwapMaxPoolPercent(const std::string& Value)
{
    if (CheckPercent(Value)) throw std::invalid_argument("The value of ZSwapMaxPoolPercent is out of range [0..100].");
    WriteLogEntry(ZSwapObject::ZSwapMaxPoolPercentName, Value, ZSwapObject::ZSwapMaxPoolPercent);
    ZSwapObject::ZSwapMaxPoolPercent = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapMaxPoolPercentName, Value);
}

std::string& ZSwapObject::GetZSwapCompressor()
{
    return ZSwapObject::ZSwapCompressor;
}

void ZSwapObject::SetZSwapCompressor(const std::string& Value)
{
    if (Value.empty()) throw std::invalid_argument("The value of ZSwapCompressor is empty.");
    WriteLogEntry(ZSwapObject::ZSwapCompressorName, Value, ZSwapObject::ZSwapCompressor);
    ZSwapObject::ZSwapCompressor = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapCompressorName, Value);
}

std::string& ZSwapObject::GetZSwapZpool()
{
    return ZSwapObject::ZSwapZpool;
}

void ZSwapObject::SetZSwapZpool(const std::string& Value)
{
    if (Value.empty()) throw std::invalid_argument("The value of ZSwapZpool is empty.");
    WriteLogEntry(ZSwapObject::ZSwapZpoolName, Value, ZSwapObject::ZSwapZpool);
    ZSwapObject::ZSwapZpool = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapZpoolName, Value);
}

std::string& ZSwapObject::GetZSwapAcceptThresholdPercent()
{
    return ZSwapObject::ZSwapAcceptThresholdPercent;
}

void ZSwapObject::SetZSwapAcceptThresholdPercent(const std::string& Value)
{
    if (CheckKernelVersion(ZSwapObject::ZSwapAcceptThresholdRequiredKernelVersion)) throw std::runtime_error("Configuring ZSwapAcceptThresholdPercent requires Linux kernel 5.6 or later.");
    if (CheckPercent(Value)) throw std::invalid_argument("The value of ZSwapAcceptThresholdPercent is out of range [0..100].");
    WriteLogEntry(ZSwapObject::ZSwapAcceptThresholdPercentName, Value, ZSwapObject::ZSwapAcceptThresholdPercent);
    ZSwapObject::ZSwapAcceptThresholdPercent = Value;
    ZSwapWorker::WriteZSwapValue(ZSwapObject::ZSwapAcceptThresholdPercentName, Value);
}

void ZSwapObject::ReadValues()
{
    ZSwapObject::ZSwapEnabled = ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapEnabledName);
    ZSwapObject::ZSwapSameFilledPages = ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapSameFilledPagesName);
    ZSwapObject::ZSwapMaxPoolPercent = ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapMaxPoolPercentName);
    ZSwapObject::ZSwapCompressor = ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapCompressorName);
    ZSwapObject::ZSwapZpool = ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapZpoolName);
    ZSwapObject::ZSwapAcceptThresholdPercent = CheckKernelVersion(ZSwapObject::ZSwapAcceptThresholdRequiredKernelVersion) ? "N/A" : ZSwapWorker::ReadZSwapValue(ZSwapObject::ZSwapAcceptThresholdPercentName);
}

ZSwapObject::ZSwapObject()
{
    ReadValues();
}
