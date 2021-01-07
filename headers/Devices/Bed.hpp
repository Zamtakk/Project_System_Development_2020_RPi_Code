#ifndef BED_HPP
#define BED_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Bed : public Device
{
public:
    Bed(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Bed();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();

private:
    void pressureSensorChange(int pressureValue);
    void buttonPress(bool buttonPressed);
    void ledStateOn(bool stateOn);
    bool ledState;
    int pressureValue;
};

#endif