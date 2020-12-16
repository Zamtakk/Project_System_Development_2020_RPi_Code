#include "Devices/Door.hpp"
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
Door::Door(string uuid, string type, SocketServer *server) : Device(uuid, type, server), aVariable(false)
{
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
string Door::getDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"aVariable", aVariable}};

    return deviceInfo.dump();
}

void Door::handleMessage(string message)
{
    socketServer->SendMessage(uuid, message);
}