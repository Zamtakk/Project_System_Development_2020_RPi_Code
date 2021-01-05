#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Website : public Device
{
public:
    Website(string uuid, string type, SocketServer *server);
    ~Website();
    string GetDeviceInfo();
    void HandleMessage(string message);
};

#endif