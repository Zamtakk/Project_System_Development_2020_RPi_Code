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

string Device::getUUID(){
    return uuid;
}

string Device::getType(){
    return type;
}

DeviceStatus Device::getStatus(){
    return status;
}