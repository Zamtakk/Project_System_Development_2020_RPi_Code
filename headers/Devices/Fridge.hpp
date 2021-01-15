#ifndef FRIDGE_HPP
#define FRIDGE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Fridge : public Device
{
public:
    Fridge(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Fridge();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool GetDoorState();

private:
    void temperatureSensorChangeInside(int value);
    void temperatureSensorChangeOutside(int value);
    void doorStateChange(bool stateOpen);
    void changeCoolingValue(int value);
    void tecStateOn(bool stateOn);
    void fanStateOn(bool stateOn);
    void updateCoolingState();
    bool doorOpen;
    bool fanState;
    bool tecState;
    int temperatureValueInside;
    int temperatureValueOutside;
    int coolingValue;
    int doorOpenTimes;
};

#endif