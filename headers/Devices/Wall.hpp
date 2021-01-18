#ifndef WALL_HPP
#define WALL_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Wall : public Device
{
public:
    Wall(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Wall();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void newLDRValue(int value);
    void openCurtain(bool p_openCurtain);
    void newDimmerValue(int value);
    void turnLedstripOn(bool p_ledstripOn);

    int LDRValue;
    int dimmerValue;
    bool curtainIsOpen;
    bool enableLamp;
    bool enableLedstrip;
};

#endif