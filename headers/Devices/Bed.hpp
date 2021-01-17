#ifndef BED_HPP
#define BED_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Bed : public Device
{
public:
    Bed(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Bed();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void buttonWasPressed(bool buttonPressed);
    void turnLedOn(bool p_ledOn);
    void newPressureSensorValue(uint16_t p_pressureValue);

    bool ledOn;
    uint8_t pressureValue;
};

#endif