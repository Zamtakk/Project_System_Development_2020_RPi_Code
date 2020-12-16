#ifndef DOOR_HPP
#define DOOR_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Door : public Device
{
public:
    Door(string uuid, string type, SocketServer *server);
    ~Door();
    string getDeviceInfo();
    void handleMessage(string message);

private:
    bool aVariable;
};

#endif