#ifndef WIB_HPP
#define WIB_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class WIB : public Device
{
public:
    WIB(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~WIB();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool isLedOn();
    bool getSwitchState();

private:
    void potValueChange(int potValue);
    void switchStateOn(bool stateOn);
    void ledStateOn(bool stateOn);

    bool ledState;
    bool switchState;
    int potValue;
};

#endif