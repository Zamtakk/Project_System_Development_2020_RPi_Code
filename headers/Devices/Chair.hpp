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
    void PressureSensorChange(string message);
    void ButtonPressed(string message);
    void LedStateOn(bool stateOn);
    void VibratorStateOn(bool stateOn);
    bool vibratorState;
    bool buttonPressed;
    bool ledState;
    int pressureValue;
};

#endif