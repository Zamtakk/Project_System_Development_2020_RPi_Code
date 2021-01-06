#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;
using std::to_string;

//TODO: website implementation, doesn't do anything yet except for being created in main

/*!
    @brief Temporary, needs implementation
    @param[in] 
    @return 
*/
Website::Website(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices)
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

/*!
    @brief Temporary, needs implementation
    @param[in] 
    @return 
*/
Website::~Website()
{
}

/*!
    @brief Packs all device variables in a JSON object, no variables implemented
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
    @brief Temporary, needs implementation
    @param[in] 
    @return 
*/
void Website::HandleMessage(string message)
{
    socketServer->SendMessage(uuid, message);
}

void Website::updateWebsite()
{
    
}
    
void Website::forwardMessage(string uuid, string message)
{

}