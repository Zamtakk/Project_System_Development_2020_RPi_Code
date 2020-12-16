#ifndef CHAIR_HPP
#define CHAIR_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Chair : public Device
{
public:
    Chair(string uuid, string type, SocketServer *server);
    ~Chair();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();
    bool IsVibratorOn();

private:
    void pressureSensorChange(int pressureValue);
    void buttonPressed(bool buttonPressed);
    bool ledStateOn(bool stateOn);
    bool vibratorStateOn(bool stateOn);
    bool ledState;
    bool vibratorState;
    bool buttonPressed;
    int  pressureValue;
};

#endif