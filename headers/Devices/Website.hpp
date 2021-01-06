#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Website : public Device
{
public:
    Website(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Website();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void updateWebsite();
    void forwardMessage(json message);
};

#endif