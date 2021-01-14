#ifndef LAMP_HPP
#define LAMP_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Lamp : public Device
{
public:
    Lamp(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Lamp();
    string GetDeviceInfo();
    void HandleMessage(string message);
    int IsLedOn();

private:
    void movementValueChange(int value);
    void ledStateUpdate(int value);
    int ledValue;
    int movementSensorValue;
};

#endif