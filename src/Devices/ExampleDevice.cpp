#include "Devices/ExampleDevice.hpp"
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
ExampleDevice::ExampleDevice(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : Device(uuid, type, server, devices), aVariable(false)
{
}

/*!
    @brief 
    @param[in] 
    @return 
*/
ExampleDevice::~ExampleDevice()
{
}

/*!
    @brief Packs all device variables in a JSON object.
    @return Returns a JSON string with all device info.
*/
string ExampleDevice::GetDeviceInfo()
{
    json deviceInfo = {
        {"UUID", uuid},
        {"Type", type},
        {"Status", status},
        {"aVariable", aVariable}};
    
    return deviceInfo.dump();
}

void ExampleDevice::HandleMessage(string message){
    socketServer->SendMessage(uuid, message);
}