
#ifndef FRAMEINFORMATION_H
#define FRAMEINFORMATION_H

#include <memory>

struct SpecificFrameInformation
{
    virtual void reset() = 0;
    virtual std::unique_ptr<SpecificFrameInformation> clone() = 0;
};

struct FrameInformation
{
    fpos_t FilePosition;
    double FirstPacketNetworkTime = 0;
    double FirstPacketDataTime = 0;
    std::shared_ptr<SpecificFrameInformation> SpecificInformation = nullptr;

    void Reset()
    {
        this->FirstPacketDataTime = 0;
        this->FirstPacketNetworkTime = 0;
        this->SpecificInformation->reset();
    }

    FrameInformation() = default;

    FrameInformation(const FrameInformation &arg) { *this = arg; }

    void operator=(const FrameInformation &arg)
    {
        this->FilePosition = arg.FilePosition;
        this->FirstPacketNetworkTime = arg.FirstPacketNetworkTime;
        this->FirstPacketDataTime = arg.FirstPacketDataTime;
        if (arg.SpecificInformation != nullptr) {
            this->SpecificInformation = arg.SpecificInformation->clone();
        }
    }
};

#endif
