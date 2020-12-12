#ifndef EXAMPLEDEVICE_HPP
#define EXAMPLEDEVICE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class ExampleDevice : public Device
{
public:
    ExampleDevice(string uuid, string type, SocketServer *server);
    ~ExampleDevice();
    string getDeviceInfo();
    void handleMessage(string message);

private:
    bool aVariable;
};

#endif