#include "Devices/Bed.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;
using std::to_string;

/*!
    @brief Constructor for the Bed object
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
*/
Bed::Bed(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices), ledState(false), pressureValue(0)
{
}

/*!
    @brief Deconstructor for Bed objects
*/
Bed::~Bed()
{
}

/*!
    @brief Function to retrieve the device information.
    @returns A string containing the device info in JSON style.
*/
string Bed::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"ledState", ledState},
        {"pressureValue", pressureValue}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Bed::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);
    if (jsonMessage["command"] == BED_FORCESENSOR_CHANGE)
    {
        pressureSensorChange((int)jsonMessage["value"]);
    }
    else if (jsonMessage["command"] == BED_BUTTON_CHANGE)
    {
        buttonPress((bool)jsonMessage["value"]);
    }
}

/*!
    @brief Function to handle incoming messages concerning changes in the pressure sensor.
    @param[in] pressureValueReceived The pressure value in the message.
*/
void Bed::pressureSensorChange(int pressureValueReceived)
{
    pressureValue = pressureValueReceived;

    map<string, Device *>::iterator it;

    for (it = devices->begin(); it != devices->end(); it++)
    {
        if (it->second->GetType() == "Website")
        {
            string senduuid = it->second->GetUUID();
            json jsonMessage = json::parse(newMessage(uuid, type, BED_FORCESENSOR_CHANGE));
            jsonMessage["value"] = pressureValueReceived;
            socketServer->SendMessage(senduuid, jsonMessage.dump());
            break;
        }
    }
}

/*!
    @brief Function to handle incoming messages when the button is pressed or released
    @param[in] buttonPressed Boolean on whether button is pressed
*/
void Bed::buttonPress(bool buttonPressed)
{
    if (ledState && buttonPressed)
    {
        ledStateOn(false);
    }
    else if (!ledState && buttonPressed)
    {
        ledStateOn(true);
    }
}

/*!
    @brief Function to turn the led on or off
    @param[in] stateOn Boolean on whether led needs to be on or off
*/
void Bed::ledStateOn(bool stateOn)
{
    ledState = stateOn;
    json jsonMessage = json::parse(newMessage(uuid, type, BED_LED_CHANGE));
    jsonMessage["value"] = ledState;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}
