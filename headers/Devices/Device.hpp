#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "CommandTypes.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Device
{
public:
    Device(string uuid, string type, SocketServer *server);
    ~Device();
    string getUUID();
    string getType();
    DeviceStatus getStatus();
    virtual string getDeviceInfo() = 0;
    virtual void handleMessage(string message) = 0;

protected:
    string uuid;
    string type;
    DeviceStatus status;
    SocketServer *socketServer;
};

#endif