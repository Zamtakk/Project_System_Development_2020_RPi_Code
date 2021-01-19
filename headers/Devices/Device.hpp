#ifndef DEVICE_HPP
#define DEVICE_HPP

// Includes

#include "CommandTypes.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>
#include <map>

// Define namespace functions

using std::map;
using std::string;

// Class definition

class Device
{
public:
    Device(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Device();
    string GetUUID();
    string GetType();
    DeviceStatus GetStatus();
    virtual string GetDeviceInfo() = 0;
    virtual void HandleMessage(string message) = 0;

protected:
    Device *getDeviceByType(string type);
    string newMessage(string p_uuid, string p_type, int p_command);
    void updateWebsite();

    string uuid;
    string type;
    DeviceStatus status;
    SocketServer *socketServer;
    map<string, Device *> *devices;
};

#endif