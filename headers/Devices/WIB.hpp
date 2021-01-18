#ifndef WIB_HPP
#define WIB_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class WIB : public Device
{
public:
    WIB(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~WIB();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void switchTurnedOn(bool p_switchOn);
    void turnLedOn(bool p_ledOn);
    void newDimmerValue(int p_dimmerValue);

    bool switchOn;
    bool ledOn;
    int dimmerValue;
};

#endif