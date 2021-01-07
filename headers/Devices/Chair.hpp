#ifndef CHAIR_HPP
#define CHAIR_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Chair : public Device
{
public:
    Chair(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Chair();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();
    bool IsVibratorOn();

private:
    void pressureSensorChange(int pressureValue);
    void buttonPress(bool buttonPressed);
    void ledStateOn(bool stateOn);
    void vibratorStateOn(bool stateOn);
    bool vibratorState;
    bool ledState;
    int pressureValue;
};

#endif