#ifndef WALL_HPP
#define WALL_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Wall : public Device
{
public:
    Wall(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Wall();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();

private:
    void lightSensorChange(int value);
    void ledStateUpdate(int value);
    void curtainsStateChange(bool stateOn);
    void potmeterChange(int value);
    void dimmerStateUpdate(string value, bool stateOn);
    int ledValue;
    int lightSensorValue;
    int potmeterValue;
    bool curtainsState;
    bool useDimmerLamp;
    bool useDimmerLedstrip;
};

#endif