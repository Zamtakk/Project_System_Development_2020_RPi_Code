#ifndef SIMULATED_DEVICE_HPP
#define SIMULATED_DEVICE_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class SimulatedDevice : public Device
{
public:
    SimulatedDevice(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~SimulatedDevice();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void buttonWasPressed(int buttonNr, bool buttonPressed);
    bool turnLedOn(int ledNr, int value);
    void newDimmerValue(int value);

    int led1Value;
    int led2Value;
    int led3Value;
    int potmeterValue;
    int activeLed;
};

#endif