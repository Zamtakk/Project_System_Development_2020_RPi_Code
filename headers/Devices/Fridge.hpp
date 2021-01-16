#ifndef FRIDGE_HPP
#define FRIDGE_HPP

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

#define THERMISTOR_NOMINAL 10000
#define NOMINAL_TEMPERATURE 25
#define TEMP_CALC_B_COEFFICIENT 3435
#define TEMP_CALC_SERIES_RESISTOR 10000

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
    float adc_to_resistance(float p_adc);
    float resistance_to_celcius(float p_resistance, uint16_t p_nominal);
    bool doorOpen;
    bool fanState;
    bool tecState;
    int temperatureValueInside;
    int temperatureValueOutside;
    int coolingValue;
    int doorOpenTimes;
};

#endif