// Includes

#include "CommandTypes.hpp"

#include "Devices/WIB.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::cout;
using std::endl;
using std::string;
using std::to_string;

// Function definitions

/*!
    @brief Constructor for the WIB object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
WIB::WIB(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      switchOn(false),
      ledOn(false),
      dimmerValue(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for WIB objects
*/
WIB::~WIB()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string WIB::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"WIB_SWITCH_ON", switchOn},
        {"WIB_LED_ON", ledOn},
        {"WIB_DIMMER_VALUE", dimmerValue}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void WIB::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        switchOn = (bool)jsonMessage["WIB_SWITCH_ON"];
        ledOn = (bool)jsonMessage["WIB_LED_ON"];
        dimmerValue = (int)jsonMessage["WIB_DIMMER_VALUE"];

        updateWebsite();
        break;
    }
    case WIB_SWITCH_ON:
    {
        switchTurnedOn((bool)jsonMessage["value"]);
        break;
    }
    case WIB_LED_ON:
    {
        turnLedOn((bool)jsonMessage["value"]);
        break;
    }
    case WIB_DIMMER_VALUE:
    {
        newDimmerValue((int)jsonMessage["value"]);
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
    @brief Handles the switch on/off event and turns on the LED
    @param[in] p_switchOn A boolean stating if the switch was turned on (true) or off (false)
*/
void WIB::switchTurnedOn(bool p_switchOn)
{
    turnLedOn(p_switchOn);

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, WIB_SWITCH_ON));
    jsonMessage["value"] = p_switchOn;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Turns the LED on or off
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
void WIB::turnLedOn(bool p_ledOn)
{
    json jsonMessage = json::parse(newMessage(uuid, type, WIB_LED_ON));
    jsonMessage["value"] = p_ledOn;

    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief A handler for new dimmer values, updates the led with a new value
    @param[in] value The new value of the dimmer
*/
void WIB::newDimmerValue(int p_dimmerValue)
{
    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, WIB_DIMMER_VALUE));
    jsonMessage["value"] = p_dimmerValue;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}
