#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;
using std::to_string;

/*!
    @brief Constructor for the website class
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
    @param[in] devices A mapping with all the connected devices
*/
Website::Website(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices)
{
    updateWebsite();
}

/*!
    @brief Deconstructor for the website class
*/
Website::~Website()
{
}

/*!
    @brief Packs all device variables in a JSON object
    @return Returns a JSON string with all device info.
*/
string Website::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status}};

    return deviceInfo.dump();
}

/*!
    @brief Message handler for incoming messages for the website
    @param[in] message The incoming message
*/
void Website::HandleMessage(string message)
{
    json jsonMessage = json::parse(message);
    if (jsonMessage["command"] == WEBSITE_UPDATE)
    {
        updateWebsite();
    }
    else if (jsonMessage["command"] == WEBSITE_FORWARD)
    {
        string type = jsonMessage["value"]["Type"];
        getDeviceByType(type)->HandleMessage(jsonMessage["value"].dump());
    }
}

/*!
    @brief Sends message to the website with updated information.
*/
void Website::updateWebsite()
{
    map<string, Device *>::iterator it;

    json jsonMessage;
    jsonMessage["UUID"] = uuid;
    jsonMessage["Type"] = type;
    jsonMessage["command"] = WEBSITE_UPDATE;

    for (it = devices->begin(); it != devices->end(); it++)
    {
        string deviceInfo = it->second->GetDeviceInfo();
        json deviceInformation = json::parse(deviceInfo);
        jsonMessage["value"].push_back(deviceInformation);
    }

    socketServer->SendMessage(uuid, jsonMessage.dump());
}