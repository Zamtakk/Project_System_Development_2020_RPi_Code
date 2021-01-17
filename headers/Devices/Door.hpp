#ifndef DOOR_HPP
#define DOOR_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Door : public Device
{
public:
    Door(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Door();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void openDoor(bool p_openDoor);
    void lockDoor(bool p_lockDoor);
    void buttonInsideWasPressed(bool buttonPressed);
    void buttonOutsideWasPressed(bool buttonPressed);
    void turnLedInsideOn(bool p_ledOn);
    void turnLedOutsideOn(bool p_ledOn);

    bool ledInsideOn;
    bool ledOutsideOn;
    bool doorIsOpen;
    bool doorIsLocked;
};

#endif