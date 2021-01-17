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
    void newPressureSensorValue(int pressureValue);
    void buttonWasPressed(bool buttonPressed);
    void turnLedOn(bool stateOn);

    bool ledOn;
    uint8_t pressureValue;
};

#endif