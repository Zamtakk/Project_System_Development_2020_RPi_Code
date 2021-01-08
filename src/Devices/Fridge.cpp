#include "Devices/Fridge.hpp"
#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

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
      temperatureValueInside(0),
      temperatureValueOutside(0),
      coolingValue(0);
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
        {"doorOpen", doorOpen},
        {"temperatureValueInside", temperatureValueInside},
        {"temperatureValueOutside", temperatureValueOutside},
        {"coolingValue", coolingValue}};

    return deviceInfo.dump();
}

/*!
    @brief Function to handle incoming messages
    @param[in] message The incoming JSON message in string format
*/
void Fridge::HandleMessage(string message)
{
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
    temperatureValueInside = value;

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURESENSORINSIDE_CHANGE));
    jsonMessage["value"] = temperatureValueInside;
    socketServer->SendMessage(uuid, jsonMessage.dump());
    
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
    temperatureValueOutside = value;

    json jsonMessage = json::parse(newMessage(uuid, type, FRIDGE_TEMPERATURESENSOROUTSIDE_CHANGE));
    jsonMessage["value"] = temperatureValueOutside;
    socketServer->SendMessage(uuid, jsonMessage.dump());
}

void Fridge::changeCoolingValue(int value){
    coolingValue = value;

}
