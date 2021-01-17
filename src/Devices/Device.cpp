// Includes

#include "Devices/Device.hpp"
#include "Websocket/SocketServer.hpp"

#include "json.hpp"
#include <string>

// Define namespace functions

using nlohmann::json;
using std::string;

// Function definitions

/*!
    @brief Parent constructor for all device objects
    @param[in] uuid The UUID of the device where the message needs to go to.
    @param[in] type The device type
	@param[in] server A pointer to the socketserver instance
	@param[in] devices A pointer to the map containing all devices
*/
Device::Device(string uuid, string type, SocketServer *server, map<string, Device *> *devices) : uuid(uuid), type(type), status(CONNECTED), socketServer(server), devices(devices)
{
}

/*!
    @brief Parent deconstructor for all device objects
*/
Device::~Device()
{
}

/*!
    @brief Return the UUID in string format.
*/
string Device::GetUUID()
{
    return uuid;
}

/*!
    @brief Return the Type in string format.
*/
string Device::GetType()
{
    return type;
}

/*!
    @brief Return the status in DeviceStatus enum format.
*/
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

/*!
    @brief Creates a new JSON message based on the given parameters
    @param[in] p_uuid The UUID for the new message
    @param[in] p_type The Type for the new message
    @param[in] p_command The Command for the new message
    @return Returns a new JSON message in string format.
*/
string Device::newMessage(string p_uuid, string p_type, int p_command)
{
    json jsonMessage;
    jsonMessage["UUID"] = p_uuid;
    jsonMessage["Type"] = p_type;
    jsonMessage["command"] = p_command;
    return jsonMessage.dump();
}