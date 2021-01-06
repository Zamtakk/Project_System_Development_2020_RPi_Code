#ifndef SIMULATED_DEVICE_HPP
#define SIMULATED_DEVICE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class SimulatedDevice : public Device
{
public:
    SimulatedDevice(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~SimulatedDevice();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool isLedOn(int ledNr);
    void buttonPress(int buttonNr, bool buttonPressed);
    bool ledStateOn(int ledNr, bool stateOn);

private:
    bool led1State;
    bool led2State;
    bool led3State;
    int potmeterValue;
};

#endif