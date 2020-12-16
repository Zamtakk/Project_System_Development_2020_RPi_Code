#include "Devices/Device.hpp"

#include "Websocket/SocketServer.hpp"

#include <string>

using std::string;

Device::Device(string uuid, string type, SocketServer *server) : uuid(uuid), type(type), status(CONNECTED), socketServer(server)
{
}

Device::~Device()
{
}

string Device::GetUUID(){
    return uuid;
}

string Device::GetType(){
    return type;
}

DeviceStatus Device::GetStatus(){
    return status;
}