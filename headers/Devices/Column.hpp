#ifndef COLUMN_HPP
#define COLUMN_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Define namespace functions

using std::string;

// Class definition

class Column : public Device
{
public:
    Column(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Column();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void newSmokeSensorValue(int value);
    void turnLedOn(bool p_ledOn);
    void turnBuzzerOn(bool p_buzzerOn);
    void buttonWasPressed(bool buttonPressed);

    bool buzzerOn;
    bool ledOn;
    int smokeValue;
    int smokeTreshold;
};

#endif