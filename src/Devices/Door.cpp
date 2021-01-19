// Includes

#include "CommandTypes.hpp"

#include "Devices/Door.hpp"
#include "Devices/Website.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;

// Function definitions

/*!
    @brief Constructor for the Door object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Door::Door(string uuid, string type, SocketServer *server, map<string, Device *> *devices)
    : Device(uuid, type, server, devices),
      ledInsideOn(false),
      ledOutsideOn(false),
      doorIsOpen(false),
      doorIsLocked(false)
{
    json jsonMessage = json::parse(newMessage(uuid, type, DEVICE_INFO));
    jsonMessage["value"] = "";
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Deconstructor for Chair objects
*/
Door::~Door()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Door::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"DOOR_LED_INSIDE_ON", ledInsideOn},
        {"DOOR_LED_OUTSIDE_ON", ledOutsideOn},
        {"DOOR_DOOR_OPEN", doorIsOpen},
        {"DOOR_DOOR_LOCKED", doorIsLocked}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Door::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);

    switch ((int)jsonMessage["command"])
    {
    case DEVICE_INFO:
    {
        ledInsideOn = (bool)jsonMessage["DOOR_LED_INSIDE_ON"];
        ledOutsideOn = (bool)jsonMessage["DOOR_LED_OUTSIDE_ON"];
        doorIsOpen = (bool)jsonMessage["DOOR_DOOR_OPEN"];

        updateWebsite();
        break;
    }
    case DOOR_BUTTON_INSIDE_PRESSED:
    {
        buttonInsideWasPressed((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_BUTTON_OUTSIDE_PRESSED:
    {
        buttonOutsideWasPressed((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_DOOR_OPEN:
    {
        openDoor((bool)jsonMessage["value"]);
        break;
    }
    case DOOR_DOOR_LOCKED:
    {
        lockDoor((bool)jsonMessage["value"]);
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
    @brief Opens or closes the door unless it is locked
    @param[in] p_openDoor A boolean stating if the door should open (true) or close (false)
*/
void Door::openDoor(bool p_openDoor)
{
    if (doorIsLocked && p_openDoor)
    {
        json jsonPacket = json::parse(newMessage(uuid, type, DOOR_DOOR_OPEN));
        jsonPacket["value"] = doorIsOpen;
        Device *site = getDeviceByType("Website");
        if (site == nullptr)
            return;
        socketServer->SendMessage(site->GetUUID(), jsonPacket.dump());
        return;
    }

    doorIsOpen = p_openDoor;

    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_DOOR_OPEN));
    jsonMessage["value"] = doorIsOpen;
    socketServer->SendMessage(uuid, jsonMessage.dump());

    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Locks or unlocks the door
    @param[in] p_openDoor A boolean stating if the lock should lock (true) or unlock (false)
*/
void Door::lockDoor(bool p_lockDoor)
{
    doorIsLocked = p_lockDoor;

    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_DOOR_LOCKED));
    jsonMessage["value"] = doorIsLocked;
    Device *website = getDeviceByType("Website");
    if (website == nullptr)
        return;

    socketServer->SendMessage(website->GetUUID(), jsonMessage.dump());
}

/*!
    @brief Handles the logic of a button press event
    @param[in] buttonPressed A boolean stating if the button was pressed (true) or released (false)
*/
void Door::buttonInsideWasPressed(bool buttonPressed)
{
    if (buttonPressed && !doorIsOpen && !doorIsLocked)
    {
        openDoor(true);
        turnLedInsideOn(true);
    }
    else if (buttonPressed && doorIsOpen)
    {
        openDoor(false);
        turnLedInsideOn(false);
    }
    else if (buttonPressed && doorIsLocked)
    {
        turnLedInsideOn(true);
    }
    else if (!buttonPressed && doorIsLocked)
    {
        turnLedInsideOn(false);
    }
}

/*!
    @brief Handles the logic of a button press event
    @param[in] buttonPressed A boolean stating if the button was pressed (true) or released (false)
*/
void Door::buttonOutsideWasPressed(bool buttonPressed)
{
    if (buttonPressed && doorIsOpen)
    {
        openDoor(false);
        turnLedOutsideOn(false);
    }
    else if (buttonPressed && doorIsLocked)
    {
        // Ring doorbell!
        turnLedOutsideOn(false);
    }
    else if (buttonPressed && !doorIsLocked && !doorIsOpen)
    {
        openDoor(true);
        turnLedOutsideOn(true);
    }
    else if(!buttonPressedOutside)
    {
        Device *column = getDeviceByType("Column");
        if (column == nullptr)
            return;

        dynamic_cast<Column*>(column)->BuzzerStateOn(false);
    }
}

/*!
    @brief Turns the inside LED on or off
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
void Door::turnLedInsideOn(bool p_ledOn)
{
    ledInsideOn = p_ledOn;
    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_LED_INSIDE_ON));
    jsonMessage["value"] = ledInsideOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

/*!
    @brief Turns the outside LED on or off
    @param[in] p_ledOn A boolean stating if the LED should turn on (true) or off (false)
*/
void Door::turnLedOutsideOn(bool p_ledOn)
{
    ledOutsideOn = p_ledOn;
    json jsonMessage = json::parse(newMessage(uuid, type, DOOR_LED_OUTSIDE_ON));
    jsonMessage["value"] = ledOutsideOn;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}
