#ifndef EXAMPLEDEVICE_HPP
#define EXAMPLEDEVICE_HPP

#include "Device.hpp"

class ExampleDevice : public Device
{
public:
    ExampleDevice(string uuid, string type);
    ~ExampleDevice();
    string getDeviceInfo();

private:
    bool aVariable;
};

#endif