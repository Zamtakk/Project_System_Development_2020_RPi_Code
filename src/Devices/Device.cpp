#include "Devices/Device.hpp"

#include <string>

using std::string;

Device::Device(string uuid, string type) : uuid(uuid), type(type), status(CONNECTED)
{
}

Device::~Device()
{
}

string Device::getUUID(){
    return uuid;
}

string Device::getType(){
    return type;
}

DeviceStatus Device::getStatus(){
    return status;
}