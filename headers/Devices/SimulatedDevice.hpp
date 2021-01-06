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
    bool ledValueUpdate(int ledNr, int value);
    void potmeterChange(int value);

private:
    int led1Value;
    int led2Value;
    int led3Value;
    int potmeterValue;
    int activeLed;
};

#endif