// Includes

#include "CommandTypes.hpp"

#include "Devices/Fridge.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>
#include <math.h>

// Define namespace functions

using nlohmann::json;
using std::cout;
using std::endl;
using std::string;
using std::to_string;

// Function definitions

/*!
    @brief Constructor for the Fridge object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Fridge::Fridge(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      doorClosed(false),
      fanOn(false),
      temperatureValueInside(0),
      temperatureValueOutside(0),
      coolerOn(false),
      requestedFridgeTemperature(19),
      doorOpenCounter(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Fridge objects
*/
Fridge::~Fridge()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Fridge::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"FRIDGE_DOOR_CLOSED", doorClosed},
        {"FRIDGE_FAN_ON", fanOn},
        {"FRIDGE_TEMPERATURE_INSIDE_VALUE", temperatureValueInside},
        {"FRIDGE_TEMPERATURE_OUTSIDE_VALUE", temperatureValueOutside},
        {"FRIDGE_COOLER_ON", coolerOn},
        {"FRIDGE_REQUESTED_FRIDGE_TEMPERATURE", requestedFridgeTemperature},
        {"FRIDGE_DOOR_OPEN_COUNTER", doorOpenCounter}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Fridge::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        doorClosed = (bool)jsonMessage["FRIDGE_DOOR_CLOSED"];
        fanOn = (bool)jsonMessage["FRIDGE_FAN_ON"];
        coolerOn = (bool)jsonMessage["FRIDGE_COOLER_ON"];
        newRawTemperatureSensorInsideValue((uint16_t)jsonMessage["FRIDGE_RAW_TEMPERATURE_SENSOR_INSIDE_VALUE"]);

        updateWebsite();
        break;
    }
    case FRIDGE_DOOR_CLOSED:
    {
        doorWasClosed((bool)jsonMessage["value"]);
        break;
    }
    case FRIDGE_RAW_TEMPERATURE_SENSOR_INSIDE_VALUE:
    {
        newRawTemperatureSensorInsideValue((uint16_t)jsonMessage["value"]);
        break;
    }
    case FRIDGE_REQUESTED_FRIDGE_TEMPERATURE:
    {
        newRequestedFridgeTemperature((int)jsonMessage["value"]);
        break;
    }
    case HEARTBEAT:
    {
        status = (DeviceStatus)jsonMessage["heartbeat"]["status"];

        updateWebsite();
        break;
    }
    default:
        break;
    }
}

/*!
    @brief Function to handle incoming messages concerning changes in the temperature sensor.
    @param[in] value The temperature value in the message.
*/
void Fridge::newRawTemperatureSensorInsideValue(uint16_t value)
{
    float resistance = adc_to_resistance((float)value);
    temperatureValueInside = (int)resistance_to_celcius((float)resistance, THERMISTOR_NOMINAL);

    updateCoolingState();

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURE_INSIDE_VALUE));
    jsonMessage["value"] = temperatureValueInside;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages concerning changes in the temperature sensor.
    @param[in] value The temperature value in the message.
*/
void Fridge::newRawTemperatureSensorOutsideValue(uint16_t value)
{
    float resistance = adc_to_resistance((float)value);
    float temperatureValueOutside = resistance_to_celcius((float)resistance, THERMISTOR_NOMINAL);

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURE_OUTSIDE_VALUE));
    jsonMessage["value"] = temperatureValueOutside;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Handles the logic of a door clode event
    @param[in] p_doorClosed A boolean stating if the door was closed (true) or opened (false)
*/
void Fridge::doorWasClosed(bool p_doorClosed)
{
    doorClosed = p_doorClosed;

    if (!doorClosed)
    {
        doorOpenCounter++;

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            return;

        json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_DOOR_OPEN_COUNTER));
        jsonMessage["value"] = doorOpenCounter;
        socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
    }
}

/*!
    @brief Saves the new requested temperature and updates the cooling
    @param[in] newTemperatureValue The new requested temperature value
*/
void Fridge::newRequestedFridgeTemperature(int newTemperatureValue)
{
    requestedFridgeTemperature = newTemperatureValue;

    updateCoolingState();
}

/*!
    @brief Turns the cooler on or off
    @param[in] p_coolerOn A boolean stating if the cooler should turn on (true) or off (false)
*/
void Fridge::turnCoolerOn(bool p_coolerOn)
{
    coolerOn = p_coolerOn;
    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_COOLER_ON));
    jsonMessage["value"] = coolerOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Turns the fan on or off
    @param[in] p_coolerOn A boolean stating if the fan should turn on (true) or off (false)
*/
void Fridge::turnFanOn(bool p_fanOn)
{
    fanOn = p_fanOn;
    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_FAN_ON));
    jsonMessage["value"] = fanOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Updates the cooling state based on the current and requested temperature.
*/
void Fridge::updateCoolingState()
{
    if (temperatureValueInside >= requestedFridgeTemperature + 1)
    {
        turnCoolerOn(true);
        turnFanOn(true);
    }
    else if (temperatureValueInside <= requestedFridgeTemperature - 1)
    {
        turnCoolerOn(false);
        turnFanOn(false);
    }
}

/*!
    @brief Part of the process of turning a raw thermistor measurement value into a temperature value. This converts the ADC value to a resistance value.
    @param[in] p_adc The raw ADC value
*/
float Fridge::adc_to_resistance(float p_adc)
{
    p_adc = 1023 - p_adc;
    p_adc += p_adc - 512;
    p_adc = 1023 / p_adc - 1;
    p_adc = TEMP_CALC_SERIES_RESISTOR / p_adc;
    return p_adc;
}

/*!
    @brief Part of the process of turning a raw thermistor measurement value into a temperature value. This converts the resistance value to a temperature value.
    @param[in] p_resistance The calculated resistance value
    @param[in] p_nominal The nominal resistance value of the thermistor at the NOMINAL_TEMPERATURE.
*/
float Fridge::resistance_to_celcius(float p_resistance, uint16_t p_nominal)
{
    float steinhart;
    steinhart = p_resistance / p_nominal;              // (R/Ro)
    steinhart = log(steinhart);                        // ln(R/Ro)
    steinhart /= TEMP_CALC_B_COEFFICIENT;              // 1/B * ln(R/Ro)
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                       // Invert
    steinhart -= 273.15;                               // convert to C
    return steinhart;
}
