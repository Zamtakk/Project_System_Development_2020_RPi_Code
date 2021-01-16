#include "Devices/Fridge.hpp"
#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>
#include <math.h>

using json = nlohmann::json;

using std::cout;
using std::endl;
using std::string;
using std::to_string;

/*!
    @brief Constructor for the Fridge object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
Fridge::Fridge(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      doorOpen(false),
      fanState(false),
      tecState(false),
      temperatureValueInside(0),
      temperatureValueOutside(0),
      coolingValue(19),
      doorOpenTimes(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICEINFO));
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
        {"doorOpenTimes", doorOpenTimes},
        {"temperatureValueInside", temperatureValueInside},
        {"temperatureValueOutside", temperatureValueOutside},
        {"coolingValue", coolingValue},
        {"tecState", tecState},
        {"fanState", fanState}};

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
    case DEVICEINFO:
    {
        doorOpen = (bool)jsonMessage["doorOpen"];
        temperatureValueInside = (int)jsonMessage["temperatureValueInside"];
        temperatureValueOutside = (int)jsonMessage["temperatureValueOutside"];
        tecState = (bool)jsonMessage["tecState"];
        fanState = (bool)jsonMessage["fanState"];

        updateCoolingState();

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
        break;
    }
    case FRIDGE_TEMPERATURESENSORINSIDE_CHANGE:
    {
        temperatureSensorChangeInside((int)jsonMessage["value"]);
        break;
    }
    case FRIDGE_COOLINGVALUE_CHANGE:
    {
        changeCoolingValue((int)jsonMessage["value"]);
        break;
    }
    case FRIDGE_SWITCH_CHANGE:
    {
        doorStateChange((bool)jsonMessage["value"]);
        break;
    }
    case HEARTBEAT:
    {
        status = (DeviceStatus)jsonMessage["heartbeat"]["status"];

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
    }
    default:
        break;
    }
}

/*!
    @brief Getter for the door state
    @returns A boolean to check whether fridge door is open or closed
*/
bool Fridge::GetDoorState()
{
    return doorOpen;
}

/*!
    @brief Function to handle incoming messages concerning changes in the temperature sensor.
    @param[in] value The temperature value in the message.
*/
void Fridge::temperatureSensorChangeInside(int value)
{
    // float resistance = adc_to_resistance((float)value);
    // float temperatureValueInside = resistance_to_celcius((float)resistance, THERMISTOR_NOMINAL);
    updateCoolingState();

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURESENSORINSIDE_CHANGE));
    jsonMessage["value"] = temperatureValueInside;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Function to handle incoming messages concerning changes in the temperature sensor.
    @param[in] value The temperature value in the message.
*/
void Fridge::temperatureSensorChangeOutside(int value)
{
    // float resistance = adc_to_resistance((float)value);
    // float temperatureValueOutside = resistance_to_celcius((float)resistance, THERMISTOR_NOMINAL);

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURESENSOROUTSIDE_CHANGE));
    jsonMessage["value"] = temperatureValueOutside;
}

void Fridge::changeCoolingValue(int value)
{
    coolingValue = value;
    updateCoolingState();
}

void Fridge::updateCoolingState()
{
    if (temperatureValueInside >= coolingValue + 1)
    {
        tecStateOn(true);
        fanStateOn(true);
    }
    else if (temperatureValueInside <= coolingValue - 1)
    {
        tecStateOn(false);
        fanStateOn(false);
    }
}

void Fridge::tecStateOn(bool stateOn)
{
    tecState = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEC_CHANGE));
    jsonMessage["value"] = tecState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

void Fridge::fanStateOn(bool stateOn)
{
    fanState = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_FAN_CHANGE));
    jsonMessage["value"] = fanState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

void Fridge::doorStateChange(bool stateOpen)
{
    doorOpen = stateOpen;

    if (stateOpen)
    {
        doorOpenTimes++;

        json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_SWITCH_CHANGE));
        jsonMessage["value"] = doorOpenTimes;

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
        {
            return;
        }
        socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
    }
}

float Fridge::adc_to_resistance(float p_adc)
{
    p_adc = 1023 - p_adc;
    p_adc += p_adc - 512;
    p_adc = 1023 / p_adc - 1;
    p_adc = TEMP_CALC_SERIES_RESISTOR / p_adc;
    return p_adc;
}

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
