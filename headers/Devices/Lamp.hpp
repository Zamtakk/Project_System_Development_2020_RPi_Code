#ifndef LAMP_HPP
#define LAMP_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Lamp : public Device
{
public:
    Lamp(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Lamp();
    string GetDeviceInfo();
    void HandleMessage(string message);
    void dimLed(int value);
    void turnLedOn(bool p_ledOn);

private:
    void newMovementDetected(bool detected);

    bool movementDetected;
    int ledValue;
    bool ledOn;
};

#endif