#ifndef COLUMN_HPP
#define COLUMN_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

class Column : public Device
{
public:
    Column(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Column();
    string GetDeviceInfo();
    void HandleMessage(string message);
    bool IsLedOn();
    void BuzzerStateOn(bool stateOn);

private:
    void changeGasTreshold(int value);
    void gasSensorChange(int value);
    void ledStateOn(bool stateOn);
    void buttonPress(bool buttonPressed);
    bool buzzerState;
    bool ledState;
    int gasSensorValue;
    int gasSensorTreshold;
};

#endif