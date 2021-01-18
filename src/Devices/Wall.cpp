// Includes

#include "CommandTypes.hpp"

#include "Devices/Wall.hpp"
#include "Devices/Lamp.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;

// Function definitions

/*!
    @brief Constructor for the Wall object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Wall::Wall(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      LDRValue(0),
      dimmerValue(0),
      curtainIsOpen(false),
      enableLamp(false),
      enableLedstrip(false)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Wall objects
*/
Wall::~Wall()
{
}

/*!
    @brief Packs all device variables in a JSON object.
    @return Returns a JSON string with all device info.
*/
string Wall::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"WALL_CURTAIN_OPEN", curtainIsOpen},
        {"WALL_LEDSTRIP_ON", enableLedstrip},
        {"WALL_LAMP_ON", enableLamp},
        {"WALL_DIMMER_VALUE", dimmerValue},
        {"WALL_LDR_VALUE", LDRValue}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Wall::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        curtainIsOpen = (bool)jsonMessage["WALL_CURTAIN_OPEN"];
        dimmerValue = (int)jsonMessage["WALL_DIMMER_VALUE"];
        LDRValue = (int)jsonMessage["WALL_LDR_VALUE"];

        updateWebsite();
        break;
    }
    case WALL_CURTAIN_OPEN:
    {
        openCurtain((bool)jsonMessage["value"]);
        break;
    }
    case WALL_LEDSTRIP_ON:
    {
        enableLedstrip = (bool)jsonMessage["value"];
        newDimmerValue(dimmerValue);
        break;
    }
    case WALL_LAMP_ON:
    {
        enableLamp = (bool)jsonMessage["value"];
        newDimmerValue(dimmerValue);
        break;
    }
    case WALL_DIMMER_VALUE:
    {
        newDimmerValue((int)jsonMessage["value"]);
        break;
    }
    case WALL_LDR_VALUE:
    {
        newLDRValue((int)jsonMessage["value"]);
        break;
    }
    case WALL_LEDSTRIP_VALUE:
    {
        dimLedstrip((int)jsonMessage["value"]);
        break;
    }
    default:
        break;
    }
}

/*!
    @brief Function to handle incoming messages concerning changes in the LDR.
    @param[in] value The new outside light value.
*/
void Wall::newLDRValue(int value)
{
    LDRValue = value;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_LDR_VALUE));
    jsonMessage["value"] = LDRValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Open or close the curtain
    @param[in] p_openCurtain A boolean stating if the curtain should be open (true) or closed (false)
*/
void Wall::openCurtain(bool p_openCurtain)
{
    curtainIsOpen = p_openCurtain;

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_CURTAIN_OPEN));
    jsonMessage["value"] = curtainIsOpen;
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
void Wall::newDimmerValue(int value)
{
    dimmerValue = value;

    if (enableLamp)
    {
        Device *lamp = getDeviceByType("Lamp");
        if (lamp == nullptr)
            return;

        dynamic_cast<Lamp *>(lamp)->turnLedOn(enableLamp);
        dynamic_cast<Lamp *>(lamp)->dimLed(dimmerValue);
    }

    if (enableLedstrip)
    {
        dimLedstrip(dimmerValue);
    }
}

/*!
    @brief Changes the dimming value of the ledstrip and updates the ledstrip state
    @param[in] value The dimming value of the ledstrip.
*/
void Wall::dimLedstrip(int value)
{
    ledstripValue = value;
    turnLedstripOn(enableLedstrip);
}

/*!
    @brief Turns the LED on or off and sets the LED to the stored dimming value
    @param[in] p_ledstripOn A boolean stating if LED should turn on (true) or off (false)
*/
void Wall::turnLedstripOn(bool p_ledstripOn)
{
    enableLedstrip = p_ledstripOn;

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_LEDSTRIP_VALUE));

    if (enableLedstrip)
    {
        jsonMessage["value"] = ledstripValue;
    }
    else
    {
        jsonMessage["value"] = 0;
    }
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    jsonMessage["value"] = ledstripValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}