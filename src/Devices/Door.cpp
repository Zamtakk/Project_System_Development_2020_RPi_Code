#include "Devices/Door.hpp"
#include "Devices/Website.hpp"
#include "Devices/Column.hpp"
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
Door::Door(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledStateInside(false),
      ledStateOutside(false),
      doorOpen(false),
      doorLocked(false)
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
Door::~Door()
{
}

/*!
    @brief Packs all device variables in a JSON object.
    @return Returns a JSON string with all device info.
*/
string Door::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"ledStateInside", ledStateInside},
        {"ledStateOutside", ledStateOutside},
        {"doorOpen", doorOpen},
        {"doorLocked", doorLocked}};

    return deviceInfo.dump();
}

void Door::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICEINFO:
    {
        ledStateInside = (bool)jsonMessage["ledStateInside"];
        ledStateOutside = (bool)jsonMessage["ledStateOutside"];
        doorOpen = (bool)jsonMessage["doorOpen"];

        Device *website = getDeviceByType("Website");
        if (website == nullptr)
            break;

        dynamic_cast<Website *>(website)->updateWebsite();
        break;
    }
    case DOOR_BUTTON1_CHANGE:
    {
        buttonPressInside((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_BUTTON2_CHANGE:
    {
        buttonPressOutside((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_LOCK_CHANGE:
    {
        changeDoorLockState((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_SERVO_CHANGE:
    {
        changeDoorState((bool)jsonMessage["value"]);
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
bool Door::IsLedOnInside()
{
    return ledStateInside;
}

/*!
    @brief Getter for the outside led state
    @returns A boolean to check whether outside led is on or off
*/
bool Door::IsLedOnOutside()
{
    return ledStateOutside;
}

void Door::changeDoorState(bool stateDoor)
{
    if (doorLocked && stateDoor)
    {
        json jsonPacket = json::parse(newMessage(uuid, type, DOOR_SERVO_CHANGE));
        jsonPacket["value"] = doorOpen;
        Device *site = getDeviceByType("Website");
        if (site == nullptr)
            return;
        socketServer->SendMessage(site->GetUUID(), jsonPacket.dump());
        return;
    }

    doorOpen = stateDoor;

    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_SERVO_CHANGE));
    jsonMessage["value"] = doorOpen;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Door::changeDoorLockState(bool doorLockState)
{
    doorLocked = doorLockState;

    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_LOCK_CHANGE));
    jsonMessage["value"] = doorLocked;
    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

void Door::buttonPressInside(bool buttonPressedInside)
{
    if (buttonPressedInside && !doorOpen && !doorLocked)
    {
        changeDoorState(true);
        ledStateOnInside(true);
    }
    else if (buttonPressedInside && doorOpen)
    {
        changeDoorState(false);
        ledStateOnInside(false);
    }
    else if (buttonPressedInside && doorLocked)
    {
        ledStateOnInside(true);
    }
    else if (!buttonPressedInside && doorLocked)
    {
        ledStateOnInside(false);
    }
}

void Door::buttonPressOutside(bool buttonPressedOutside)
{
    if (buttonPressedOutside && doorOpen)
    {
        changeDoorState(false);
        ledStateOnOutside(false);
    }
    else if (buttonPressedOutside && doorLocked)
    {
        ledStateOnOutside(false);

        Device *column = getDeviceByType("Column");
        if (column == nullptr)
            return;

        dynamic_cast<Column*>(column)->BuzzerStateOn(true);
    }
    else if (buttonPressedOutside && !doorLocked && !doorOpen)
    {
        changeDoorState(true);
        ledStateOnOutside(true);
    }
    else if(!buttonPressedOutside)
    {
        Device *column = getDeviceByType("Column");
        if (column == nullptr)
            return;

        dynamic_cast<Column*>(column)->BuzzerStateOn(false);
    }
}

void Door::ledStateOnInside(bool stateOn)
{
    ledStateInside = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_LED1_CHANGE));
    jsonMessage["value"] = ledStateInside;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

void Door::ledStateOnOutside(bool stateOn)
{
    ledStateOutside = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_LED2_CHANGE));
    jsonMessage["value"] = ledStateOutside;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}