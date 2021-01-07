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
    bool isLedOn(int ledNr);
    bool getSwitchState(int switchNr);

private:
    void potValueChange(int potValue);
    void swtichStateOn(int switchNr, bool stateOn);
    void ledStateOn(int LedNr, bool stateOn);

    bool ledState0;
    bool ledState1;
    bool ledState2;
    bool ledState3;
    int potValue;
};

#endif