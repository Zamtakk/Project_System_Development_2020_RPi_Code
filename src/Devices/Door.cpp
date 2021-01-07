#include "Devices/Door.hpp"
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
Door::Door(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledStateInside(false),
      ledStateOutside(false),
      doorState(false),
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
        {"doorState", doorState},
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
        doorState = (bool)jsonMessage["doorState"];
        doorLocked = (bool)jsonMessage["doorLocked"];

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
    doorState = stateDoor;

    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_SERVO_CHANGE));
    jsonMessage["value"] = doorState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

void Door::changeDoorLockState(bool doorLockState)
{
    doorLocked = doorLockState;
}

void Door::buttonPressInside(bool buttonPressedInside)
{
    if (doorState && buttonPressedInside)
    {
        changeDoorState(false);
    }
    else if (!doorState && buttonPressedInside && !doorLocked)
    {
        changeDoorState(true);
    }
    if (ledStateInside && buttonPressedInside)
    {
        ledStateOnInside(false);
        ledStateOnOutside(false);
    }
    else if (!ledStateInside && buttonPressedInside)
    {
        ledStateOnInside(true);
        ledStateOnOutside(true);
    }
}

void Door::buttonPressOutside(bool buttonPressedOutside)
{
    if (doorState && buttonPressedOutside)
    {
        changeDoorState(false);
    }
    else if (!doorState && buttonPressedOutside && !doorLocked)
    {
        changeDoorState(true);
    }
    else if (!doorState && buttonPressedOutside && doorLocked)
    {
        //TODO: make call to ring the doorbell
    }
    if (ledStateInside && buttonPressedOutside)
    {
        ledStateOnInside(false);
        ledStateOnOutside(false);
    }
    else if (!ledStateInside && buttonPressedOutside)
    {
        ledStateOnInside(true);
        ledStateOnOutside(true);
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