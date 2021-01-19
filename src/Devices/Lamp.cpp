// Includes

#include "CommandTypes.hpp"

#include "Devices/Lamp.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;

// Function definitions

/*!
    @brief Constructor for the Lamp object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Lamp::Lamp(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      movementDetected(false),
      ledValue(0),
      ledOn(false)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Lamp objects
*/
Lamp::~Lamp()
{
}

/*!
    @brief Packs all device variables in a JSON object.
    @return Returns a JSON string with all device info.
*/
string Lamp::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"LAMP_MOVEMENT_DETECTED", movementDetected},
        {"LAMP_LED_VALUE", ledValue},
        {"LAMP_LED_ON", ledOn}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Lamp::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        movementDetected = (int)jsonMessage["LAMP_MOVEMENT_DETECTED"];
        ledValue = (int)jsonMessage["LAMP_LED_VALUE"];

        if (ledValue > 0)
        {
            ledOn = true;
        }
        else
        {
            ledOn = false;
        }

        updateWebsite();
        break;
    }
    case LAMP_MOVEMENT_DETECTED:
    {
        newMovementDetected((bool)jsonMessage["value"]);
        break;
    }
    case LAMP_LED_VALUE:
    {
        dimLed((int)jsonMessage["value"]);
        break;
    }
    case LAMP_LED_ON:
    {
        turnLedOn((bool)jsonMessage["value"]);
        break;
    }
    case HEARTBEAT:
    {
        status = (DeviceStatus)jsonMessage["heartbeat"]["status"];

        updateWebsite();
    }
    default:
        break;
    }
}

/*!
    @brief Handles the movement detected event
    @param[in] detected A boolean stating if movement was detected (true) or not (false)
*/
void Lamp::newMovementDetected(bool detected)
{
    movementDetected = detected;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, LAMP_MOVEMENT_DETECTED));
    jsonMessage["value"] = movementDetected;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Changes the dimming value of the led and updates the led state
    @param[in] value The dimming value of the led.
*/
void Lamp::dimLed(int value)
{
    ledValue = value;
    turnLedOn(ledOn);
}

/*!
    @brief Turns the LED on or off and sets the LED to the stored dimming value
    @param[in] p_ledOn A boolean stating if LED should turn on (true) or off (false)
*/
void Lamp::turnLedOn(bool p_ledOn)
{
    ledOn = p_ledOn;

    json jsonMessage = json::parse(newMessage(uuid, type, LAMP_LED_VALUE));

    if (ledOn)
    {
        jsonMessage["value"] = ledValue;
    }
    else
    {
        jsonMessage["value"] = 0;
    }
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    jsonMessage["value"] = ledValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());

    jsonMessage = json::parse(newMessage(uuid, type, LAMP_LED_ON));
    jsonMessage["value"] = ledOn;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}
