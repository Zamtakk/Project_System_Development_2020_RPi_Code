#include "Devices/Wall.hpp"
#include "Devices/Website.hpp"
#include "Devices/Lamp.hpp"
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
Wall::Wall(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledValue(0),
      lightSensorValue(0),
      potmeterValue(0),
      curtainsState(false)
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
        {"ledValue", ledValue},
        {"lightSensorValue", lightSensorValue},
        {"potmeterValue", potmeterValue},
        {"curtainsState", curtainsState},
        {"useDimmerLamp", useDimmerLamp},
        {"useDimmerLedstrip", useDimmerLedstrip}};

    return deviceInfo.dump();
}

void Wall::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICEINFO:
    {
        ledValue = (int)jsonMessage["ledValue"];
        lightSensorValue = (int)jsonMessage["lightSensorValue"];
        potmeterValue = (int)jsonMessage["potmeterValue"];
        curtainsState = (bool)jsonMessage["curtainsState"];

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
        break;
    }
    case WALL_LEDSTRIP_CHANGE:
    {
        ledStateUpdate((int)jsonMessage["value"]);
        break;
    }
    case WALL_LDR_CHANGE:
    {
        lightSensorChange((int)jsonMessage["value"]);
        break;
    }
    case WALL_SCREEN_CHANGE:
    {
        curtainsStateChange((bool)jsonMessage["value"]);
        break;
    }
    case WALL_POTMETER_CHANGE:
    {
        potmeterChange((int)jsonMessage["value"]);
        break;
    }
    case WALL_LAMPDIMMER_CHANGE:
    {
        dimmerStateUpdate("lamp", jsonMessage["value"]);
        break;
    }
    case WALL_LEDSTRIPDIMMER_CHANGE:
    {
        dimmerStateUpdate("ledstrip", jsonMessage["value"]);
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
bool Wall::IsLedOn()
{
    if (ledValue > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Wall::lightSensorChange(int value)
{
    lightSensorValue = value;

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_LDR_CHANGE));
    jsonMessage["value"] = lightSensorValue;
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Wall::ledStateUpdate(int value)
{
    ledValue = value;

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_LEDSTRIP_CHANGE));
    jsonMessage["value"] = ledValue;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Wall::curtainsStateChange(bool stateOn)
{
    curtainsState = stateOn;

    json jsonMessage = json::parse(newMessage(uuid, type, WALL_SCREEN_CHANGE));
    jsonMessage["value"] = curtainsState;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
    {
        return;
    }
    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Wall::potmeterChange(int value)
{
    potmeterValue = value;

    if (useDimmerLamp)
    {
        Device *lamp = getDeviceByType("Lamp");
        if (lamp == nullptr)
            return;

        dynamic_cast<Lamp *>(lamp)->ledStateUpdate(potmeterValue);
    }
    else if (useDimmerLedstrip)
    {
        ledStateUpdate(potmeterValue);
    }
}

void Wall::dimmerStateUpdate(string value, bool stateOn)
{
    if (value == "lamp")
    {
        useDimmerLamp = stateOn;
    }
    else if (value == "ledstrip")
    {
        useDimmerLedstrip = stateOn;
    }
}