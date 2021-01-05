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
Website::Website(string uuid, string type, SocketServer *server) : Device(uuid, type, server)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
Website::~Website()
{
}

/*!
    @brief Packs all device variables in a JSON object.
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

void Website::HandleMessage(string message)
{
    socketServer->SendMessage(uuid, message);
}