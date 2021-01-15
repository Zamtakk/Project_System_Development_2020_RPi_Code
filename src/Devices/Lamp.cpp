#include "Devices/Lamp.hpp"
#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;

/*!
    @brief 
    @param[in] 
    @return 
*/
Lamp::Lamp(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledDimValue(0),
      ledOn(false),
      movementSensorValue(0)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICEINFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief 
    @param[in] 
    @return 
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
        {"ledOn", ledOn},
        {"ledDimValue", ledDimValue},
        {"movementSensorValue", movementSensorValue}};

    return deviceInfo.dump();
}

void Lamp::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICEINFO:
    {
        ledDimValue = (int)jsonMessage["ledValue"];
        movementSensorValue = (int)jsonMessage["movementSensorValue"];
        if (ledDimValue > 0)
        {
            ledOn = true;
        }
        else
        {
            ledOn = false;
        }

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
        break;
    }
    case LAMP_LED_DIMMER_CHANGE:
    {
        ledStateUpdate(ledOn, (int)jsonMessage["value"]);
        break;
    }
    case LAMP_LED_ONOFF_CHANGE:
    {
        ledStateUpdate((bool)jsonMessage["value"], ledDimValue);
        break;
    }
    case LAMP_MOVEMENTSENSOR_CHANGE:
    {
        movementValueChange((int)jsonMessage["value"]);
        break;
    }
    default:
        break;
    }
}

/*!
    @brief Getter for the inside led state
    @returns A boolean to check whether inside led is on or off
*/
bool Lamp::IsLedOn()
{
    return ledOn;
}

void Lamp::movementValueChange(int value)
{
    movementSensorValue = value;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }

    json jsonMessage = json::parse(newMessage(uuid, type, LAMP_MOVEMENTSENSOR_CHANGE));
    jsonMessage["value"] = movementSensorValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Lamp::ledStateUpdate(bool stateOn, int value)
{
    ledDimValue = value;
    ledOn = stateOn;

    json jsonMessage = json::parse(newMessage(uuid, type, LAMP_LED_DIMMER_CHANGE));

    if (ledOn)
    {
        jsonMessage["value"] = ledDimValue;
    }
    else
    {
        jsonMessage["value"] = 0;
    }
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }
    jsonMessage["value"] = ledDimValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());

    jsonMessage = json::parse(newMessage(uuid, type, LAMP_LED_ONOFF_CHANGE));
    jsonMessage["value"] = ledOn;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}