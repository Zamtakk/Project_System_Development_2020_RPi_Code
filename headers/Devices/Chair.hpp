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
    void PressureSensorChange();
    void ButtonPressed();
    bool LedStateOn();
    bool VibratorStateOn();
    bool ledState;
    bool vibratorState;
    bool buttonPressed;
    int  pressureValue;
};

#endif