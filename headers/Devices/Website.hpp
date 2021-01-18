#ifndef WEBSITE_HPP
#define WEBSITE_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Website : public Device
{
public:
    Website(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Website();
    string GetDeviceInfo();
    void HandleMessage(string message);
    void update();
};

#endif