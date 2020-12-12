#include "Device.hpp"

#include <string>

using std::string;

Device::Device(string uuid, string type) : uuid(uuid), type(type), status(CONNECTED)
{
}

Device::~Device()
{
}