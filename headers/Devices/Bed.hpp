#ifndef BED_HPP
#define BED_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Bed : public Device
{
public:
    Bed(string uuid, string type, SocketServer *server);
    ~Bed();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();

private:
    void pressureSensorChange(int pressureValue);
    void buttonPressed(bool buttonPressed);
    bool ledStateOn(bool stateOn);
    bool ledState;
    bool buttonPressed;
    int pressureValue;
};

#endif