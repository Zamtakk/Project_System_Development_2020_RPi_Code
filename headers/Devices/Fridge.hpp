#ifndef FRIDGE_HPP
#define FRIDGE_HPP

// Includes

#include "Device.hpp"
#include "Websocket/SocketServer.hpp"

#include <string>

// Defines

#define THERMISTOR_NOMINAL 10000
#define NOMINAL_TEMPERATURE 25
#define TEMP_CALC_B_COEFFICIENT 3435
#define TEMP_CALC_SERIES_RESISTOR 10000

// Define namespace functions

using std::string;

// Class definition

class Fridge : public Device
{
public:
    Fridge(string uuid, string type, SocketServer *server, map<string, Device *> *devices);
    ~Fridge();
    string GetDeviceInfo();
    void HandleMessage(string message);

private:
    void newRawTemperatureSensorInsideValue(int value);
    void newRawTemperatureSensorOutsideValue(int value);
    void doorWasClosed(bool p_doorClosed);
    void newRequestedFridgeTemperature(int newTemperatureValue);
    void turnCoolerOn(bool p_coolerOn);
    void turnFanOn(bool p_fanOn);
    void updateCoolingState();

    float adc_to_resistance(float p_adc);
    float resistance_to_celcius(float p_resistance, uint16_t p_nominal);

    bool doorClosed;
    bool fanOn;
    int temperatureValueInside;
    int temperatureValueOutside;
    bool coolerOn;
    int requestedFridgeTemperature;
    int doorOpenCounter;
};

#endif