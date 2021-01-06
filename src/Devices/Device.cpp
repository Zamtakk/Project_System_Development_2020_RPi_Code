#include "Devices/Device.hpp"

#include "Websocket/SocketServer.hpp"

#include "json.hpp"

#include <string>

using json = nlohmann::json;

using std::string;

Device::Device(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : uuid(uuid), type(type), status(CONNECTED), socketServer(server), devices(devices)
{
}

Device::~Device()
{
}

string Device::GetUUID()
{
    return uuid;
}

string Device::GetType()
{
    return type;
}

DeviceStatus Device::GetStatus()
{
    return status;
}

/*!
    @brief Searches though all created devices and returns the devices that was requested based on the type name
    @param[in] type A string defining which type should be searched for.
    @return Returns a Device pointer pointing to the first device found that fits the requested device type.
*/
Device *Device::getDeviceByType(string type)
{
    map<string, Device *>::iterator it;

    for (it = devices->begin(); it != devices->end(); it++)
    {
        if (it->second->GetType().compare(type) == 0)
        {
            return it->second;
        }
    }
    return nullptr;
}

string Device::newMessage(string p_uuid, string p_type, int p_command)
{
    json jsonMessage;
    jsonMessage["UUID"] = p_uuid;
    jsonMessage["Type"] = p_type;
    jsonMessage["command"] = p_command;
    return jsonMessage.dump();
}