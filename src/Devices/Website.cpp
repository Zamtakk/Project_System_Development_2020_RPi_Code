#include "Devices/Website.hpp"
#include "CommandTypes.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;

//TODO: website implementation, doesn't do anything yet except for being created in main

/*!
    @brief Temporary, needs implementation
    @param[in] 
    @return 
*/
Website::Website(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices)
{
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