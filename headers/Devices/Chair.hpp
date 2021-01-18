#ifndef CHAIR_HPP
#define CHAIR_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Chair : public Device
{
public:
    Chair(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Chair();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void buttonWasPressed(bool buttonPressed);
    void turnLedOn(bool p_ledOn);
    void turnVibratorOn(bool p_vibratorOn);
    void newPressureSensorValue(uint8_t p_pressureValue);

    bool ledOn;
    bool vibratorOn;
    uint8_t pressureValue;
};

#endif