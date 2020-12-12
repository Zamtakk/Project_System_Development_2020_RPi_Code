#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "CommandTypes.hpp"

#include <string>

using std::string;

class Device
{
public:
    Device(string uuid, string type);
    ~Device();
    virtual string getDeviceInfo() = 0;

protected:
    string uuid;
    string type;
    DeviceStatus status;
};

#endif