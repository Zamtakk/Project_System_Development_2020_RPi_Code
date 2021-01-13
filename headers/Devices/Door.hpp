#ifndef DOOR_HPP
#define DOOR_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Door : public Device
{
public:
    Door(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Door();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOnInside();
    bool IsLedOnOutside();

private:
    void changeDoorState(bool doorState);
    void changeDoorLockState(bool doorLockState);
    void buttonPressInside(bool buttonPressedInside);
    void buttonPressOutside(bool buttonPressedOutside);
    void ledStateOnInside(bool stateOn);
    void ledStateOnOutside(bool stateOn);
    bool ledStateInside;
    bool ledStateOutside;
    bool doorOpen;
    bool doorLocked;
};

#endif